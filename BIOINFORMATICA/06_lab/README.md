# Hierarchical Clustering

Performs agglomerative clustering using single, complete, and average linkage.

## Input

A lower triangular distance matrix in a text file. Example (`distancia.txt` with 4 elements):

```

0
1.0 0
2.0 1.5 0
3.0 2.5 1.2 0

````

## Usage

```bash
python main.py distancia.txt
````

Outputs:

* `single_linkage.txt`, `complete_linkage.txt`, `average_linkage.txt`
* `single_dendrogram.png`, `complete_dendrogram.png`, `average_dendrogram.png`

## Requirements

* Python 3.x
* numpy, matplotlib, scipy

Install with:

```bash
pip install numpy matplotlib scipy
```