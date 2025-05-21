# DropMerge Game 🎮

**DropMerge Game** é um jogo de terminal feito em linguagem C no qual peças com valores numéricos caem em um grid. O objetivo é **fundir peças com o mesmo valor** para formar números maiores, evitando que o tabuleiro fique completamente cheio.

---

## 🛠️ Tecnologias Utilizadas

- Linguagem: **C**
- Bibliotecas padrão:
  - `stdio.h`
  - `stdlib.h`
  - `string.h`

---

## 🧠 Mecânica do Jogo

- Peças numéricas caem no grid.
- Quando duas peças com o **mesmo valor colidem**, elas **se fundem** em uma de valor maior.
- O jogador deve manter o tabuleiro sob controle e evitar que ele fique completamente cheio.
- O jogo termina quando não há mais espaço para novas peças.

---

## 📦 Estrutura do Projeto

```
DropMerge-Game/
├── sprites/              # Representações visuais (ASCII ou arquivos auxiliares)
├── src/                  # Código-fonte em C
│   ├── main.c
│   └── outros arquivos .c/.h
├── README.md             # Este arquivo
└── ...
```

---

## 🚀 Como Compilar e Executar

### 🧰 Pré-requisitos

- Compilador C (GCC ou compatível)
- Terminal Linux, macOS ou Windows com suporte a ANSI (cmd, PowerShell, Git Bash, etc.)

### 🔧 Compilação

No terminal, dentro da pasta do projeto, execute:

```bash
gcc -o dropmerge src/main.c
```

> (Adapte esse comando se houver múltiplos arquivos .c)

### ▶️ Execução

Após compilar, rode:

```bash
./dropmerge
```

---

## 🎮 Controles

> (Se aplicável — substitua pelos reais)

- **A / D**: mover peças para esquerda/direita
- **S / Barra de espaço**: acelerar a queda
- **Q**: sair do jogo

---

## 📸 Imagens (opcional)

> Inclua aqui prints do terminal rodando o jogo (pode usar `Ctrl+Shift+Print` e colar aqui futuramente)

---

## 📄 Licença

Este projeto está sob a licença [MIT] — sinta-se livre para estudar, modificar e compartilhar.

---

Desenvolvido por [Seu Nome ou Equipe] 💡
