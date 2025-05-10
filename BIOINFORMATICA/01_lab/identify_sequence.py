def identify_sequence(sequence):
    """
    Identifies if the given sequence is DNA, RNA, or a protein.
    Priority: DNA > RNA > Protein > Unknown.
    """
    if not sequence:
        return "Invalid (empty sequence)"

    sequence = sequence.upper()
    unique_chars = set(sequence)

    # Conjuntos de caracteres
    dna_bases = {'A', 'T', 'C', 'G'}
    rna_bases = {'A', 'U', 'C', 'G'}
    amino_acids = {
        'A', 'R', 'N', 'D', 'C', 'E', 'Q', 'G', 'H', 'I',
        'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V',
        'B', 'Z', 'X', '*'
    }
    all_allowed = dna_bases | rna_bases | amino_acids

    # Verificar caracteres inválidos primero
    if not unique_chars.issubset(all_allowed):
        return "Invalid (unrecognized characters)"

    # ---- Prioridad DNA/RNA sobre proteínas ----
    # Si todos los caracteres son bases de DNA/RNA, es DNA o RNA
    if unique_chars.issubset(dna_bases | rna_bases):
        has_u = 'U' in unique_chars
        has_t = 'T' in unique_chars

        if has_u and has_t:
            return "Invalid (cannot have both U and T)"
        elif has_u:
            return "RNA"
        elif has_t:
            return "DNA"
        else:
            return "DNA"  # Solo A/C/G (es DNA por defecto)
    
    # Si no es DNA/RNA, verificar si es proteína
    elif unique_chars.issubset(amino_acids):
        return "Protein"
    
    # Si hay mezcla (ej: ATCGX), es Unknown
    else:
        return "Unknown (could be modified protein or invalid sequence)"

def translate_dna_to_amino_acids(dna_sequence):
    """
    Translates a DNA sequence into amino acids using codons.
    
    Parameters:
        dna_sequence (str): The DNA sequence to translate.
    
    Returns:
        str: The corresponding amino acids.
    """
    codon_table = {
        'ATA': 'I', 'ATC': 'I', 'ATT': 'I', 'ATG': 'M',
        'ACA': 'T', 'ACC': 'T', 'ACG': 'T', 'ACT': 'T',
        'AAC': 'N', 'AAT': 'N', 'AAA': 'K', 'AAG': 'K',
        'AGC': 'S', 'AGT': 'S', 'AGA': 'R', 'AGG': 'R',
        'CTA': 'L', 'CTC': 'L', 'CTG': 'L', 'CTT': 'L',
        'CCA': 'P', 'CCC': 'P', 'CCG': 'P', 'CCT': 'P',
        'CAC': 'H', 'CAT': 'H', 'CAA': 'Q', 'CAG': 'Q',
        'CGA': 'R', 'CGC': 'R', 'CGG': 'R', 'CGT': 'R',
        'GTA': 'V', 'GTC': 'V', 'GTG': 'V', 'GTT': 'V',
        'GCA': 'A', 'GCC': 'A', 'GCG': 'A', 'GCT': 'A',
        'GAC': 'D', 'GAT': 'D', 'GAA': 'E', 'GAG': 'E',
        'GGA': 'G', 'GGC': 'G', 'GGG': 'G', 'GGT': 'G',
        'TCA': 'S', 'TCC': 'S', 'TCG': 'S', 'TCT': 'S',
        'TTC': 'F', 'TTT': 'F', 'TTA': 'L', 'TTG': 'L',
        'TAC': 'Y', 'TAT': 'Y', 'TAA': '*', 'TAG': '*',
        'TGC': 'C', 'TGT': 'C', 'TGA': '*', 'TGG': 'W',
    }

    if len(dna_sequence) < 2:
        return ""
    
    codons = [dna_sequence[i:i+3] for i in range(0, len(dna_sequence), 3) if len(dna_sequence[i:i+3]) == 3]
    amino_acids = [codon_table.get(codon, '?') for codon in codons]
    
    return ''.join(amino_acids)