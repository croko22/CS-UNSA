import unittest
from smith_waterman import smith_waterman

class TestBioinformaticsUtils(unittest.TestCase):

    def test_smith_waterman_basic(self):
        S1 = "TACGCGC"
        S2 = "TCCGA"
        result = smith_waterman(S1, S2, match=1, mismatch=-1, gap=-2)
        self.assertEqual(result['final_score'], 2)
        self.assertGreaterEqual(result['num_alignments'], 1)
        # Check that at least one alignment is a valid local alignment (CG)
        alignments = result['alignments']
        found = False
        for a1, a2 in alignments:
            if "CG" in a1.replace("-", "") and "CG" in a2.replace("-", ""):
                found = True
        self.assertTrue(found)

    def test_smith_waterman_perfect_match(self):
        S1 = "GATTACA"
        S2 = "GATTACA"
        result = smith_waterman(S1, S2)
        self.assertEqual(result['final_score'], 7)
        self.assertIn(("GATTACA", "GATTACA"), result['alignments'])

    def test_smith_waterman_no_match(self):
        S1 = "AAAA"
        S2 = "TTTT"
        result = smith_waterman(S1, S2)
        self.assertEqual(result['final_score'], 0)
        self.assertEqual(result['num_alignments'], 0)