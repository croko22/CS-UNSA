import unittest
from identify_sequence import identify_sequence, translate_dna_to_amino_acids

class TestIdentifySequence(unittest.TestCase):
    def test_dna_sequences(self):
        self.assertEqual(identify_sequence("ATCG"), "DNA")  # Caso básico
        self.assertEqual(identify_sequence("atcg"), "DNA")  # Lowercase
        self.assertEqual(identify_sequence("AAAAA"), "DNA")  # Homopolímero
        self.assertEqual(identify_sequence("GATTACA"), "DNA")  # Secuencia con 'T'
        self.assertEqual(identify_sequence("ACGT" * 100), "DNA")  # Secuencia larga

    def test_rna_sequences(self):
        self.assertEqual(identify_sequence("AUCG"), "RNA")  # Caso básico
        self.assertEqual(identify_sequence("aucg"), "RNA")  # Lowercase
        self.assertEqual(identify_sequence("UUUUU"), "RNA")  # Homopolímero
        self.assertEqual(identify_sequence("GAUUACA"), "RNA")  # Secuencia con 'U'

    def test_protein_sequences(self):
        self.assertEqual(identify_sequence("MFTW"), "Protein")  # Caso básico
        self.assertEqual(identify_sequence("mftw"), "Protein")  # Lowercase
        self.assertEqual(identify_sequence("ACDEFGHIKLMNPQRSTVWY"), "Protein")  # Todos AA estándar
        self.assertEqual(identify_sequence("X"), "Protein")  # Aminoácido ambiguo
        self.assertEqual(identify_sequence("BZ"), "Protein")  # AA no estándar (Asp/Asn, Glu/Gln)

    def test_invalid_cases(self):
        self.assertEqual(identify_sequence(""), "Invalid (empty sequence)")  # Secuencia vacía
        self.assertEqual(identify_sequence("ATUB"), "Unknown (could be modified protein or invalid sequence)")  # Mezcla DNA/RNA
        self.assertEqual(identify_sequence("123"), "Invalid (unrecognized characters)")  # Números
        self.assertEqual(identify_sequence("@#!"), "Invalid (unrecognized characters)")  # Símbolos
        self.assertEqual(identify_sequence("HOLA"), "Invalid (unrecognized characters)") # HI
        self.assertEqual(identify_sequence("AUCGT"), "Invalid (cannot have both U and T)")  # Mezcla RNA/DNA

    def test_edge_cases(self):
        self.assertEqual(identify_sequence("G" * 1000), "DNA")  # Secuencia repetitiva larga
        self.assertEqual(identify_sequence("U" * 1000), "RNA")  # Secuencia repetitiva larga
        self.assertEqual(identify_sequence("L" * 1000), "Protein")  # Secuencia repetitiva larga

if __name__ == "__main__":
    unittest.main()