# Análisis de Cadenas Biológicas

Este proyecto implementa algoritmos para analizar cadenas biológicas y determinar si son ADN, ARN o proteínas. Además, identifica los aminoácidos presentes en las cadenas de proteínas y traduce cadenas de ADN en aminoácidos.

## Descripción

- **Cadenas extraídas de:**  
  [DDBJ Search](https://ddbj.nig.ac.jp/arsa/search?lang=en&cond=quick_search&query=o&operator=AND)

- **Funcionalidades principales:**
  1. Identificar si una cadena es:
     - ADN
     - ARN
     - Proteína
  2. Si es una proteína, indicar los aminoácidos presentes.
  3. Si es ADN, traducirlo en tripletes (codones) y determinar los aminoácidos correspondientes.

- **Pruebas:**  
  Las pruebas se realizan utilizando el framework `xUnit`.

## Requisitos

- Python 3.8 o superior
- Librerías necesarias:
  - `requests` (para descargar archivos GenBank)

Instala las dependencias con:

```bash
pip install -r requirements.txt