🧮 Hindley-Milner Type Checking

> 📚 A C++ implementation of the Hindley-Milner type inference system for statically analyzing expressions using polymorphic type inference.

---

## 🧠 What is Hindley-Milner?

The **Hindley-Milner type system** is a classical algorithm used in statically typed functional programming languages (like Haskell and ML). It enables:

* **Polymorphic type inference**
* **Strong static typing** without requiring explicit annotations
* **Type generalization** and **unification**

This project brings these concepts to life in **pure C++**.

---

## 📂 Project Structure

```
Hindley-Milner-Type-Checking/
├── PPL A-3/
│   ├── Danish_Final.cc       # Main type inference implementation
│   ├── inputbuf.cc           # Input buffer abstraction
│   ├── inputbuf.h
│   ├── lexer.h               # Lexer for parsing tokens
│   └── metadata.yml          # Metadata for documentation or CI
├── README.md                 # Project documentation
└── .gitattributes            # Git settings
```

---

## 🧪 What It Does

This project:

* Parses input expressions (likely lambda calculus or similar)
* Applies the Hindley-Milner type inference algorithm
* Outputs the inferred type or a type error
* Uses **unification** and **substitution** techniques for inference

---

## 🏁 How to Compile and Run

### 🔧 Compile the project:

```bash
g++ Danish_Final.cc inputbuf.cc -o hm_type_checker
```

### ▶️ Run with input:

```bash
./hm_type_checker < input.txt
```

Where `input.txt` contains expressions such as:

```plaintext
let id = \x -> x in id 5
```

---

## 📘 Example Input & Output

| 📝 Input                    | 🧾 Output         |
| --------------------------- | ----------------- |
| `\x -> x`                   | `∀α. α → α`       |
| `\x -> \y -> x`             | `∀α β. α → β → α` |
| `let id = \x -> x in id id` | `∀α. α → α`       |

---

## 🛠️ Core Concepts Implemented

| 🔧 Feature         | ✅ Description                                        |
| ------------------ | ---------------------------------------------------- |
| **Lexer**          | Tokenizes lambda calculus-like syntax                |
| **Parser**         | Reads expressions and generates abstract syntax tree |
| **Type Inference** | Performs unification and generalization              |
| **Polymorphism**   | Supports generic types with type variables           |

---

## 📦 Requirements

* C++11 or newer
* Linux/macOS/WSL (recommended)
* Basic knowledge of type systems and lambda calculus is helpful
