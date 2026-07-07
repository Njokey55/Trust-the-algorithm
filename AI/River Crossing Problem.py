
from collections import deque

ALL   = frozenset({'M', 'L', 'G', 'R'}) # immutable set representing all entities
NAMES = {'M': 'Man', 'L': 'Leopard', 'G': 'Goat', 'R': 'Grass'}

# State: (frozenset of left bank, boat_side)
# Goal : left bank is empty (everyone crossed to the right)


def bank_str(bank): # converts set into readable text 
    if not bank:
        return "(empty)"
    return ", ".join(NAMES[e] for e in sorted(bank))


def is_safe(left, boat_side): # check if anything gets eaten on the unattended bank
    "Return True if no entity gets eaten on the unattended bank."
    unattended = (ALL - left) if boat_side == 'left' else left 
    if 'M' in unattended:
        return True
    if 'L' in unattended and 'G' in unattended:
        return False   # Leopard eats Goat
    if 'G' in unattended and 'R' in unattended:
        return False   # Goat eats Grass
    return True


def successors(state): # generate all valid moves from the current state
    "Yield (next_state, action_label) for all safe moves."
    left, boat = state
    right      = ALL - left
    from_bank  = left if boat == 'left' else right # determines departure bank based on boat's current position
    to_side    = 'right' if boat == 'left' else 'left' # boat's destination side is opposite of its current position

    if 'M' not in from_bank: # boat must have man to operate
        return

    cargos = [frozenset(['M'])]
    for e in from_bank: # loop through entities on the departure bank and create possible cargo combinations
        if e != 'M': # ignore man since he's already included in the cargo
            cargos.append(frozenset(['M', e])) # add cargo 

    for cargo in cargos: # try every possible boat trip 
        new_left  = (left - cargo) if boat == 'left' else (left | cargo) # calculate new left bank state after the move
        new_state = (new_left, to_side) # create new state
        if is_safe(new_left, to_side): # only keep safe states
            passenger = sorted(cargo - {'M'}) # find who travels with the man
            whom      = NAMES[passenger[0]] if passenger else "nobody" # name passenger 
            arrow     = "-> right" if boat == 'left' else "<- left"
            label     = "Man takes {} ({})".format(whom, arrow)
            yield new_state, label # yield allows the function to return a generator that produces a sequence of valid nexxt states 


def is_goal(state): # goal test 
    left, _ = state
    return len(left) == 0


#BFS

def bfs():
    start     = (frozenset({'M', 'L', 'G', 'R'}), 'left')
    queue     = deque([(start, [start], [])]) # create a queue for BFS with initial state, path, and actions
    visited   = {start} # prevent revisiting states by keeping track of visited states
    explored  = []
    level_map = {start: 0}

    print("BFS - Breadth-First Search")

    while queue: # continue until queue is empty
        state, path, actions = queue.popleft() # remove front state
        lvl = level_map[state]
        explored.append((lvl, state, actions[-1] if actions else "Start"))

        if is_goal(state): # stop if solution is found
            print("\n  BFS examined {} states before finding the goal.\n".format(len(explored)))
            print(" BFS Traversal Log (every state dequeued) ")
            print("  {:<4} {:<6} {:<24} {:<24}  Operation".format(
                "#", "Level", "Left Bank", "Right Bank"))
            print("  " + "-"*85)
            for i, (lv, s, act) in enumerate(explored):
                l, b  = s
                r     = ALL - l
                goal_tag = "  <-- GOAL" if is_goal(s) and i > 0 else ""
                print("  {:<4} {:<6} {:<24} {:<24}  {}{}".format(
                    i, lv, bank_str(l), bank_str(r), act, goal_tag))
            print("\n BFS Solution Path (shortest) ")
            _print_solution(path, actions)
            return path, actions

        for nxt, label in successors(state): # generate all safe next states
            if nxt not in visited:# avoids lopps
                visited.add(nxt)
                level_map[nxt] = lvl + 1
                queue.append((nxt, path + [nxt], actions + [label]))# add child state to back of queue

    print("  No solution found.")
    return None, None


#DFS

def dfs():
    start   = (frozenset({'M', 'L', 'G', 'R'}), 'left')
    dfs_log = []

    print(" DFS - Depth-First Search")

    def _dfs(state, path, actions, visited, depth): # DFS is implemented recursively
        action = actions[-1] if actions else "Start"
        dfs_log.append({ # stores traversal information 
            'depth': depth, 'state': state,
            'action': action, 'goal': False, 'backtrack': False
        })

        if is_goal(state): # stops if solution is found
            dfs_log[-1]['goal'] = True
            return path, actions

        found_move = False
        for nxt, label in successors(state):# try every child
            if nxt not in visited:
                found_move = True
                result = _dfs(nxt, path + [nxt], actions + [label], # move deeper into search tree
                              visited | {nxt}, depth + 1)
                if result[0]:
                    return result

        if not found_move: # if no further moves are possible, it backtracks to explore other branches
            dfs_log.append({ # record backtracking step for analysis
                'depth': depth, 'state': state,
                'action': "[BACKTRACK] No moves at depth {}".format(depth),
                'goal': False, 'backtrack': True
            })
        return None, None

    path, actions = _dfs(start, [start], [], {start}, 0)

    forward    = [e for e in dfs_log if not e['backtrack']]
    backtracks = [e for e in dfs_log if e['backtrack']]

    print("\n  DFS made {} forward moves and {} backtracks.\n".format(
        len(forward), len(backtracks)))
    print(" DFS Traversal Log (indentation shows depth) ")
    print("  {:<4} {:<6} {:<24} {:<24}  Operation".format(
        "#", "Depth", "Left Bank", "Right Bank"))
    print("  " + "-"*85)
    for i, entry in enumerate(dfs_log):
        l, b     = entry['state']
        r        = ALL - l
        indent   = "  " * entry['depth']
        goal_tag = "  <-- GOAL" if entry['goal'] else ""
        bt_tag   = "  [BACKTRACK]" if entry['backtrack'] else ""
        l_str    = bank_str(l) if not entry['backtrack'] else "---"
        r_str    = bank_str(r) if not entry['backtrack'] else "---"
        print("  {:<4} {:<6} {:<24} {:<24}  {}{}{}{}".format(
            i, entry['depth'], l_str, r_str,
            indent, entry['action'], goal_tag, bt_tag))

    if path:
        print("\n DFS Solution Path (first path found) ")
        _print_solution(path, actions)
    else:
        print("  No solution found.")

    return path, actions


def _print_solution(path, actions): # displays the solution path in a readable format
    print()
    print("  {:<5} {:<36} {:<28} {:<8} {}".format(
        "Step", "Action", "Left Bank", "Boat", "Right Bank"))
    print("  " + "-"*95)
    for i, state in enumerate(path):
        left, boat = state
        right  = ALL - left
        action = actions[i - 1] if i > 0 else "Initial state"
        goal_tag = "  <== GOAL" if is_goal(state) else ""
        print("  {:<5} {:<36} {:<28} {:<8} {}{}".format(
            i, action, bank_str(left), boat, bank_str(right), goal_tag))
    print("\n  Total moves: {}".format(len(actions)))


# Main

if __name__ == "__main__":
    print("  PROBLEM 1 - RIVER CROSSING")

    bfs_path, bfs_actions = bfs()
    dfs_path, dfs_actions = dfs()

    print("  COMPARISON SUMMARY")
    print("  BFS moves : {}  (guaranteed shortest)".format(
        len(bfs_actions) if bfs_actions else "N/A"))
    print("  DFS moves : {}  (first path found)".format(
        len(dfs_actions) if dfs_actions else "N/A"))