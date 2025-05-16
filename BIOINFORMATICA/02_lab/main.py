import unittest
from utils import is_substring, needleman_wunsch

class TestBioinformaticsUtils(unittest.TestCase):

    def test_is_substring(self):
        self.assertTrue(is_substring("TACG", "TACGCGC"))
        self.assertTrue(is_substring("TCCGA", "TCCGA--"))
        self.assertFalse(is_substring("AAA", "TTT"))

    def test_needleman_wunsch(self):
        S1 = "TACGCGC"
        S2 = "TCCGA"
        result = needleman_wunsch(S1, S2, match=1, mismatch=-1, gap=-2)
        self.assertEqual(result['final_score'], -1)
        self.assertEqual(result['num_alignments'], 1)
        expected_alignments = [
            ("TACGCGC", "T-C-CGA"),
        ]
        generated_alignments = [(a1, a2) for a1, a2 in result['alignments']]
        for align in expected_alignments:
            self.assertIn(align, generated_alignments)

if __name__ == '__main__':
    unittest.main()