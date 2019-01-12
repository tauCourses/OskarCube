
puzzle_size = [11,8,6]
start = [2,5,4]
end = [6,4,5]
filename = 'a'

def puzzle_path(puzzle_size, start, end):
    pass

def create_puzzle(puzzle_size, puzzle_path):
    pass


path = puzzle_path(puzzle_size, start, end)
xy, yz, zx = create_puzzle(puzzle_size, path)

with open(filename, 'w') as f:
    f.write('%d %d %d\n', puzzle_size[0], puzzle_size[1], puzzle_size[2])
