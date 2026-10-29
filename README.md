# CLI Web Browser Simulator

A lightweight command-line web browser simulator written in C, developed as a Data Structures and Algorithms (SDA) university coursework project.

The project models the core operational mechanics of modern web browsers—such as managing multiple tabs, tracking page history, handling downloads, and managing cache—using fundamental data structures.

---

## Features & Architecture

The simulator implements browser features using custom data structures:

* **Tab Management (`Doubly Linked List`)**:
  * Create (`newtab`), switch (`change_tab`), and close (`close`) tabs.
  * Preserves relative tab ordering and current active tab reference.

* **Navigation & Page History (`Stacks`)**:
  * Forward and backward navigation (`back`, `forward`) using two stack structures per tab.
  * Visiting a new URL pushes to the back stack and resets the forward history.

* **Cache & Resources**:
  * Simulates caching downloaded resources and history entries.
  * Page structure includes URL, resource content, and size.

* **Memory Safety**:
  * Full deallocation of dynamically allocated nodes, stacks, and strings on program termination.

---

## Repository Structure

```text
.
├── main.c        # Core browser implementation and entry point
└── README.md     # Project documentation
```

---

## Getting Started

### Prerequisites

A modern C compiler supporting C99 or higher (e.g., `gcc` or `clang`):

```bash
sudo apt update
sudo apt install build-essential
```

### Compilation

Compile the project using standard flags:

```bash
gcc -Wall -Wextra -std=c99 main.c -o browser
```

### Execution

Run the simulator interactively or feed commands via input redirection:

```bash
# Interactive mode
./browser

# File input redirection
./browser < input.txt
```

---

## Example Commands

| Command | Description |
| :--- | :--- |
| `goto <url>` | Navigates the current tab to `<url>` |
| `newtab` | Opens a new empty tab |
| `closetab` | Closes the active tab |
| `change_tab <id>` | Switches active focus to tab `<id>` |
| `back` | Goes back to the previous page in history |
| `forward` | Advances to the next page in forward history |
| `history` | Displays current tab's browsing history |

---

## Author

* Developed as part of the **Data Structures and Algorithms** course.