# **Star Alignment Tool**  
**A parallelized implementation of the Star Alignment algorithm for multiple sequence alignment (MSA)**  

---

## **📌 Overview**  
This tool performs **multiple sequence alignment (MSA)** using the **Star Alignment** algorithm, which:  
1. Computes pairwise alignments between all sequences.  
2. Selects a **"star sequence"** (the one with the highest total alignment score).  
3. Aligns all other sequences against the star sequence.  
4. Produces a **consensus multiple sequence alignment (MSA)**.  

The implementation is **parallelized using OpenMP** for faster execution on multi-core CPUs.  

---

## **⚙️ Features**  
✅ **Needleman-Wunsch global alignment** (with configurable scoring)  
✅ **Parallel computation** of the score matrix using OpenMP  
✅ **Star selection** based on highest total alignment score  
✅ **Multiple Sequence Alignment (MSA) generation**  
✅ **Output includes**:  
   - Pairwise score matrix  
   - Alignments against the star sequence  
   - Final MSA in FASTA-like format  

---

## **📥 Installation & Requirements**  
### **Dependencies**  
- **C++17** (or higher)  
- **OpenMP** (for parallelization)  
- **Make** (for compilation)  

### **Compilation**  
```bash
make          # Builds the executable in ./build/main
make clean    # Cleans build files
```

---

## **🚀 Usage**  
### **Basic Execution**  
```bash
./build/main input.fasta
```
This generates an output file: `input.fasta.out.txt`  

### **Output File Contents**  
1. **Score Matrix** (pairwise alignment scores)  
2. **Alignments vs. Star Sequence** (each sequence aligned to the star)  
3. **Final MSA** (multiple sequence alignment)  

### **Example Command**  
```bash
./build/main examples/proteins.fasta
```
Output saved to: `examples/proteins.fasta.out.txt`  

---

## **🔧 Configuration (Optional)**  
Modify `main.cpp` to adjust alignment parameters:  
```cpp
const int match_score = 1;      // Score for matching characters
const int mismatch_score = -1;  // Penalty for mismatches
const int gap_score = -2;       // Penalty for gaps
```

---

## **📊 Example Output**  
### **1. Score Matrix**  
|       | Seq1 | Seq2 | Seq3 |  
|-------|------|------|------|  
| **Seq1** | 0    | 120  | 95   |  
| **Seq2** | 120  | 0    | 110  |  
| **Seq3** | 95   | 110  | 0    |  

### **2. Star Sequence Selection**  
```
[*] Star sequence: Seq2
```

### **3. Alignments Against Star**  
```
>Seq1  
ATG--CGT  
ATGG-CGT  

>Seq3  
A-TGCCGT  
AT-GCCGT  
```

### **4. Final MSA**  
```
>Seq1  
ATG--CGT  
>Seq2  
ATGG-CGT  
>Seq3  
AT-GCCGT  
```

---

## **📈 Performance Notes**  
- **Parallelized** for faster execution (scales with CPU cores).  
- **Optimal for small-to-medium datasets** (for large datasets, consider progressive alignment methods).  

---

## **🔗 References**  
1. **Needleman-Wunsch Algorithm** (1970) - Global pairwise alignment.  
2. **Star Alignment** - A heuristic for MSA.  