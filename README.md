# Battleships-Tues-25

## 📜 Game Overview

**Battleships-Tues-25** is a two-player game (also includes a single-player mode vs AI) inspired by the classic "Battleship" board game. Each player places ships on a 10x10 grid and then takes turns trying to sink the opponent's fleet by guessing their ship positions.

---

## 🛳️ Rules and Gameplay

### 🔧 Ship Placement Rules

- Ships **must not touch each other**, not even diagonally.
- You have **10 ships** in total:
  - 1 **Flagship** (length 6)
  - 2 **Warships** (length 4)
  - 3 **Destroyers** (length 3)
  - 4 **Boats** (length 2)

### 🧭 Placement Options

- Ships can be placed by:
  - Loading from a configuration file
  - Manually entering positions and directions via console input
- After placing each ship, the player may:
  - Add another ship (if fewer than 10 are placed)
  - Edit the position of a ship
  - Preview the current board

Invalid configurations (e.g. overlapping ships, invalid directions, or duplicate ship types) are rejected with appropriate error messages.

---

## 🎮 Game Flow

### 🔁 Turn-Based System

- Players take turns guessing coordinates on the enemy's board.
- After a **hit**, the same player goes again.
- After a **miss**, the turn switches.
- Players may guess by:
  - Entering absolute coordinates (e.g., `B5`)
  - Selecting a direction relative to the last guess (up/down/left/right)

### 🎯 Tracking Progress

- Players can view:
  - All previous guesses and results
  - Ships they’ve successfully hit and sunk

### 🏁 End of Game

- The game ends when one player sinks **all 10 ships** of their opponent.

---

## 🤖 Single Player Mode

- A random ship layout is generated for the AI.
- The AI takes turns firing at the player’s board using simple decision logic.

---

## 💾 Save & Load Game Replays

- After a game ends, players can **save the game replay**.
- Replays are stored in an **encrypted file** using a **password** entered by the player.
- Saved replays can be loaded and visualized at any time.

---

## 🔐 Encrypted Save Files

- Game replays are encrypted with a password provided at the time of saving.
- Replays can only be viewed by entering the correct password during loading.

---


## ✅ Features Summary

- ✅ Full two-player mode
- ✅ Single-player mode with AI
- ✅ Ship validation (no overlap, adjacency)
- ✅ Replay saving & viewing
- ✅ File encryption with password
- ✅ Clean CLI interface

---