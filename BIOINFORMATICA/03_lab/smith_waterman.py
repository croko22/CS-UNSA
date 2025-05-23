def smith_waterman(seq1: str, seq2: str, match: int = 1, mismatch: int = -1, gap: int = -2):
    m, n = len(seq1), len(seq2)
    score_matrix = [[0] * (n + 1) for _ in range(m + 1)]
    traceback = [[[] for _ in range(n + 1)] for _ in range(m + 1)]

    max_score = 0
    max_pos = []

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            diag = score_matrix[i-1][j-1] + (match if seq1[i-1] == seq2[j-1] else mismatch)
            up = score_matrix[i-1][j] + gap
            left = score_matrix[i][j-1] + gap
            score = max(0, diag, up, left)
            score_matrix[i][j] = score
            if score == 0:
                continue
            if score == diag:
                traceback[i][j].append('D')
            if score == up:
                traceback[i][j].append('U')
            if score == left:
                traceback[i][j].append('L')
            if score > max_score:
                max_score = score
                max_pos = [(i, j)]
            elif score == max_score:
                max_pos.append((i, j))

    alignments = []
    def backtrack(i, j, align1, align2):
        if score_matrix[i][j] == 0:
            alignments.append((align1[::-1], align2[::-1]))
            return
        for direction in traceback[i][j]:
            if direction == 'D':
                backtrack(i-1, j-1, align1 + seq1[i-1], align2 + seq2[j-1])
            elif direction == 'U':
                backtrack(i-1, j, align1 + seq1[i-1], align2 + '-')
            elif direction == 'L':
                backtrack(i, j-1, align1 + '-', align2 + seq2[j-1])

    for i, j in max_pos:
        backtrack(i, j, '', '')

    arrow_map = {'D': '↖', 'U': '↑', 'L': '←'}
    arrow_matrix = []
    for row in traceback:
        arrow_row = []
        for cell in row:
            if not cell:
                arrow_row.append('.')
            else:
                arrow_row.append(''.join(arrow_map[d] for d in cell))
        arrow_matrix.append(arrow_row)

    return {
        'final_score': max_score,
        'score_matrix': score_matrix,
        'arrow_matrix': arrow_matrix,
        'num_alignments': len(alignments),
        'alignments': alignments
    }

def save_smith_waterman_output(result, filename="output.txt"):
    with open(filename, "w") as f:
        f.write(f"Final Score: {result['final_score']}\n")
        f.write("Score Matrix:\n")
        for row in result['score_matrix']:
            f.write(str(row) + "\n")
        f.write("Arrow Matrix:\n")
        for row in result['arrow_matrix']:
            f.write(str(row) + "\n")
        f.write("Alignments:\n")
        for a1, a2 in result['alignments']:
            f.write(a1 + "\n")
            f.write(a2 + "\n\n")
        f.write(f"Number of Alignments: {result['num_alignments']}\n")