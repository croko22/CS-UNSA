from smith_waterman import smith_waterman, save_smith_waterman_output


S1 = "TACGCGC"
S2 = "TCCGA"    
result = smith_waterman(S1, S2, match=1, mismatch=-1, gap=-2)
save_smith_waterman_output(result, "output.txt")