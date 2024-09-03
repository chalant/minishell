# MINISHELL
A bash clone
## Description
This project aims to reproduce a subset of bash's capabilities for education purposes.
  - ### Key features:
    - Implements the Earley Parser making it capable of parsing all context-free grammars.
    - supports pipe "|", logical AND "&&", logical OR "||" and parenthesis "()"
    - supports redirections ">, <, >>, <<" and wildcards "*"
## Table of Contents
1. [Requirements](#installation)
2. [Installation](#requirements)
3. [Usage](#usage)

## Requirements
- **readline**: Required for running the project on MacOS
## Installation
### On Ubuntu
1. **Clone repository:**
```
git clone https://github.com/chalant/minishell.git
```
2. **Build project:**
```
make
```
### On Mac OS

1. **Clone repository:**
```
git clone https://github.com/chalant/fdf.git
```
2. **Install dependencies**
```
brew install readline
```
4. **Build project:**
```
make
```
## Usage
```
./minishell
```
## Example
```
./minishell
((echo "Starting process" && mkdir project) || echo "Failed to create directory") && (cd project && touch file.txt)
```
