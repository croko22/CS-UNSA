from utils import is_substring, needleman_wunsch, save_needleman_wunsch_output

S1 = "TACGCGC"
S2 = "TCCGA"
result = needleman_wunsch(S1, S2, match=1, mismatch=-1, gap=-2)
print("Final Score:", result['final_score'])
print("Score Matrix:")
for row in result['score_matrix']:
    print(row)
print("Number of Alignments:", result['num_alignments'])
for align in result['alignments']:
    print("Alignment:")
    print(align[0])
    print(align[1])
    print()

save_needleman_wunsch_output(result, 'output.txt')