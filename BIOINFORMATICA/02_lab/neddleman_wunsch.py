def is_substring(seq1: str, seq2: str) -> bool:
    return seq1 in seq2 or seq2 in seq1

def needleman_wunsch(seq1: str, seq2: str, match: int = 1, mismatch: int = -1, gap: int = -2):
    m, n = len(seq1), len(seq2)
    score_matrix = [[0] * (n + 1) for _ in range(m + 1)]
    traceback = [[[] for _ in range(n + 1)] for _ in range(m + 1)]

    for i in range(1, m + 1):
        score_matrix[i][0] = i * gap
        traceback[i][0] = ['U']
    for j in range(1, n + 1):
        score_matrix[0][j] = j * gap
        traceback[0][j] = ['L']

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            diag = score_matrix[i-1][j-1] + (match if seq1[i-1] == seq2[j-1] else mismatch)
            up = score_matrix[i-1][j] + gap
            left = score_matrix[i][j-1] + gap
            max_score = max(diag, up, left)
            score_matrix[i][j] = max_score
            if diag == max_score:
                traceback[i][j].append('D')
            if up == max_score:
                traceback[i][j].append('U')
            if left == max_score:
                traceback[i][j].append('L')

    alignments = []
    def backtrack(i, j, align1, align2):
        if i == 0 and j == 0:
            alignments.append((align1[::-1], align2[::-1]))
            return
        for direction in traceback[i][j]:
            if direction == 'D':
                backtrack(i-1, j-1, align1 + seq1[i-1], align2 + seq2[j-1])
            elif direction == 'U':
                backtrack(i-1, j, align1 + seq1[i-1], align2 + '-')
            elif direction == 'L':
                backtrack(i, j-1, align1 + '-', align2 + seq2[j-1])

    backtrack(m, n, '', '')

    # Convertir la matriz de flechitas a una representación de texto
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
        'final_score': score_matrix[m][n],
        'score_matrix': score_matrix,
        'arrow_matrix': arrow_matrix,
        'num_alignments': len(alignments),
        'alignments': alignments
    }

def save_needleman_wunsch_output(result: dict, filename: str):
    """
    Guarda los resultados del alineamiento Needleman-Wunsch en un archivo .txt.

    Args:
        result (dict): Resultado del alineamiento.
        filename (str): Nombre del archivo de salida.
    """
    with open(filename, 'w') as f:
        f.write(f"Score final: {result['final_score']}\n\n")
        f.write("Matriz de scores:\n")
        for row in result['score_matrix']:
            f.write('\t'.join(map(str, row)) + '\n')
        f.write("\nMatriz de flechas (trazado):\n")
        for row in result['arrow_matrix']:
            f.write('\t'.join(row) + '\n')
        f.write(f"\nCantidad de alineamientos: {result['num_alignments']}\n\n")
        f.write("Alineamientos generados:\n")
        for align1, align2 in result['alignments']:
            f.write(align1 + '\n')
            f.write(align2 + '\n')
            f.write('\n')