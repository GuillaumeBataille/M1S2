# Step 1: Read data from file
import os
path = os.path.dirname(os.path.abspath(__file__))+"/minimax_tree.txt"

class Node:
    def __init__(self, depth, value, position, player):
        self.depth = depth
        self.value = value
        self.position = position
        self.player = player
        self.children = []

# Step 1: Read data from file
with open(path, 'r') as f:
    data = f.readlines()

# Step 2: Parse each line and create node objects
nodes = []
for line in data:
    depth = int(line.split(':')[0])
    value = int(line.split(' ')[1][6:])
    position = int(line.split(' ')[2][9:])
    player = int(line.split(' ')[3][7:])
    node = Node(depth, value, position, player)
    nodes.append(node)

# Step 3: Create dictionary to store nodes
node_dict = {}
for node in nodes:
    if node.depth not in node_dict:
        node_dict[node.depth] = []
    node_dict[node.depth].append(node)

# Step 4: Create parent-child relationships
for i in range(len(nodes)):
    for j in range(i+1, len(nodes)):
        if nodes[j].depth == nodes[i].depth + 1:
            if (nodes[j].position-1)//3 == (nodes[i].position-1):
                nodes[i].children.append(nodes[j])
            elif (nodes[i].position-1)//3 == (nodes[j].position-1):
                nodes[j].children.append(nodes[i])

# Step 5: Draw the tree
def draw_tree(node, prefix=''):
    print(f"{prefix}Node (depth={node.depth}): value={node.value}, position={node.position}, player={node.player}")
    for i, child in enumerate(node.children):
        if i == len(node.children) - 1:
            draw_tree(child, prefix + '    ')
        else:
            draw_tree(child, prefix + '│   ')
    return

for depth in sorted(node_dict.keys()):
    print(f"Depth {depth}:")
    for node in node_dict[depth]:
        draw_tree(node)