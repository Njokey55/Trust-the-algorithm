
from collections import deque

J3 = 3   # capacity of 3-litre jug
J4 = 4   # capacity of 4-litre jug

# State: (amount_in_3L_jug, amount_in_4L_jug)


def is_goal(state): # goal test checks if either jug has exactly 2 litres
    return state[0] == 2 or state[1] == 2


def fill_bar(val, cap): # creates visual bar for jug levels e.g. [##.] for 2/3
    """ASCII progress bar e.g. [##..] for 2/4."""
    return "[" + "#" * val + "." * (cap - val) + "]"


def state_str(state): # extract jug levels from state and return a string representation
    j3, j4 = state
    return "3L:{}{:d}L   4L:{}{:d}L".format(
        fill_bar(j3, J3), j3, fill_bar(j4, J4), j4)


def successors(state): # yield all valid jug operations
    """Yield (next_state, operation_label) for every valid jug operation."""
    j3, j4 = state
    ops = [] # creates an empty list to store valid operations and their resulting states

    # 1. Fill from tap
    if j3 < J3:
        ops.append(((J3, j4),
            "Fill 3L jug from tap       (3L: {}->{}  4L: {})".format(j3, J3, j4)))
    if j4 < J4:
        ops.append(((j3, J4),
            "Fill 4L jug from tap       (3L: {}  4L: {}->{})".format(j3, j4, J4)))

    # 2. Empty to ground
    if j3 > 0:
        ops.append(((0, j4),
            "Empty 3L jug to ground     (3L: {}->0  4L: {})".format(j3, j4)))
    if j4 > 0:
        ops.append(((j3, 0),
            "Empty 4L jug to ground     (3L: {}  4L: {}->0)".format(j3, j4)))

    # 3. Pour 3L -> 4L
    if j3 > 0 and j4 < J4:
        pour = min(j3, J4 - j4)
        ops.append(((j3 - pour, j4 + pour),
            "Pour {}L: 3L->4L jug          (3L: {}->{}  4L: {}->{})".format(
                pour, j3, j3 - pour, j4, j4 + pour)))

    # 4. Pour 4L -> 3L
    if j4 > 0 and j3 < J3:
        pour = min(j4, J3 - j3)
        ops.append(((j3 + pour, j4 - pour),
            "Pour {}L: 4L->3L jug          (3L: {}->{}  4L: {}->{})".format(
                pour, j3, j3 + pour, j4, j4 - pour)))

    return ops


# BFS 

def bfs():
    start     = (0, 0)
    queue     = deque([(start, [start], [])]) # create a queue for BFS with initial state, path, and actions
    visited   = {start} # prevent revisiting states by keeping track of visited states
    explored  = []
    level_map = {start: 0}

    print("  BFS - Breadth-First Search")

    while queue: #runs until the queue is empty
        state, path, actions = queue.popleft() #dequeue the next state to explore
        lvl = level_map[state]
        explored.append((lvl, state, actions[-1] if actions else "Start"))

        if is_goal(state): #check if the current state is the goal state
            print("\n  BFS examined {} states to find the goal.\n".format(len(explored)))
            print("  BFS Traversal Log (every state dequeued) ")
            print("  {:<4} {:<6} {:<14} {:<14}  Operation".format(
                "#", "Level", "3L Jug", "4L Jug"))
            print("  " + "-"*72)
            for i, (lv, s, act) in enumerate(explored):
                j3, j4   = s
                goal_tag = "  <-- GOAL" if is_goal(s) and i > 0 else ""
                print("  {:<4} {:<6} {:<14} {:<14}  {}{}".format(
                    i, lv,
                    "{}L{}".format(j3, fill_bar(j3, J3)),
                    "{}L{}".format(j4, fill_bar(j4, J4)),
                    act, goal_tag))
            print("\n  BFS Solution Path (shortest) ")
            _print_solution(path, actions)
            return path, actions
        parent = {}

        for nxt, label in successors(state): # generate all possible next states and their corresponding action labels
            if nxt not in visited: #only explore unseen states to avoid cycles
                visited.add(nxt)
                level_map[nxt] = lvl + 1
                queue.append((nxt, path + [nxt], actions + [label])) # adds child states to the queue with updated path and actions
    print("  No solution found.")
    return None, None


# DFS

def dfs(): # explores as deep as possible along each branch before backtracking
    start   = (0, 0)
    dfs_log = []

    print("  DFS - Depth-First Search")

    def _dfs(state, path, actions, visited, depth): # recursive helper function for DFS that takes the current state, path, actions, visited states, and depth level as arguments
        # each recursive call goes deeper into the search tree 
        action = actions[-1] if actions else "Start"
        dfs_log.append({ # stores traversal information for logging purposes
            'depth': depth, 'state': state,
            'action': action, 'goal': False, 'backtrack': False
        })

        if is_goal(state): # stops recursion when a goal state is found
            dfs_log[-1]['goal'] = True
            return path, actions

        found_move = False
        for nxt, label in successors(state): # visists every possible successor state from the current state
            if nxt not in visited:
                found_move = True
                result = _dfs(nxt, path + [nxt], actions + [label],
                              visited | {nxt}, depth + 1) # moves deeper into the state space
                if result[0]:
                    return result

        if not found_move: # if no further moves are possible, it backtracks to explore other branches
            dfs_log.append({ # logs backtracking steps for analysis
                'depth': depth, 'state': state,
                'action': "[BACKTRACK] dead end at depth {}".format(depth),
                'goal': False, 'backtrack': True
            })
        return None, None

    path, actions = _dfs(start, [start], [], {start}, 0)

    forward    = [e for e in dfs_log if not e['backtrack']]
    backtracks = [e for e in dfs_log if e['backtrack']]

    print("\n  DFS made {} forward moves and {} backtracks.\n".format(
        len(forward), len(backtracks)))
    print("  DFS Traversal Log (indentation shows depth) ")
    print("  {:<4} {:<6} {:<14} {:<14}  Operation".format(
        "#", "Depth", "3L Jug", "4L Jug"))
    print("  " + "-"*72)
    for i, entry in enumerate(dfs_log):
        j3, j4   = entry['state']
        indent   = "  " * entry['depth']
        goal_tag = "  <-- GOAL" if entry['goal'] else ""
        bt_tag   = "  [BACKTRACK]" if entry['backtrack'] else ""
        j3_str   = "{}L{}".format(j3, fill_bar(j3, J3)) if not entry['backtrack'] else "---"
        j4_str   = "{}L{}".format(j4, fill_bar(j4, J4)) if not entry['backtrack'] else "---"
        print("  {:<4} {:<6} {:<14} {:<14}  {}{}{}{}".format(
            i, entry['depth'], j3_str, j4_str,
            indent, entry['action'], goal_tag, bt_tag))

    if path:
        print("\n  DFS Solution Path (first path found) ")
        _print_solution(path, actions)
    else:
        print("  No solution found.")

    return path, actions


def _print_solution(path, actions): # displays the solution path 
    print()
    print("  {:<5} {:<14} {:<14}  Operation".format("Step", "3L Jug", "4L Jug"))
    print("  " + "-"*72)
    for i, state in enumerate(path):
        j3, j4 = state
        op     = actions[i - 1] if i > 0 else "Initial state"
        b3     = "{}L{}".format(j3, fill_bar(j3, J3))
        b4     = "{}L{}".format(j4, fill_bar(j4, J4))
        goal_tag = "  <== GOAL: 2L measured!" if is_goal(state) and i > 0 else ""
        print("  {:<5} {:<14} {:<14}  {}{}".format(i, b3, b4, op, goal_tag))
    print("\n  Total steps: {}".format(len(actions)))


# Main

if __name__ == "__main__":
    print("  PROBLEM 2 - WATER JUG PROBLEM")

    bfs_path, bfs_actions = bfs()
    dfs_path, dfs_actions = dfs()

    
    print("  COMPARISON SUMMARY")
    print("  BFS steps : {}  (optimal - fewest operations)".format(
        len(bfs_actions) if bfs_actions else "N/A"))
    print("  DFS steps : {}  (first path explored)".format(
        len(dfs_actions) if dfs_actions else "N/A"))
    print()
    print("  KEY DIFFERENCE:")
    print("  BFS is level-by-level -> always finds shortest path.")
    print("  DFS goes deep first   -> may find a longer path.")