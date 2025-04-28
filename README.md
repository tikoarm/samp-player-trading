# SA-MP Player Trading System 💬

A player-to-player trading system for SA-MP RPG servers, written in Pawn.  
This system allows players to securely offer, negotiate, and finalize trades for items, resources, or services in-game.

## 🎮 What Is This?

This system introduces a real-time trading mechanism where players can create offers and negotiate directly with each other.  
Through a clean dialog-based interface, players can send offers, accept, reject, or modify deals — enhancing interaction and economy on the server.

## 🧩 Features

- Real-time player-to-player trading
- Dialog-based UI for easy interaction
- Safe offer creation, acceptance, and cancellation
- Trade security checks to prevent abuse
- Hook-based event handling for clean server integration
- Modular structure for flexibility and future expansion

## 🗂 File Structure

- `main.inc` – Entry point and system initialization
- `functions.inc` – Core trade management logic
- `hooks.inc` – Server event hooks (player disconnect, etc.)
- `hooks_define.inc` – Hook definitions and macros
- `vars.inc` – Shared variables (trade state, player offers)
- `dialogs.inc` – Dialog interfaces for trading interactions

## 🧱 Technologies Used

- **Pawn** – Primary scripting language
- **SA-MP 0.3.7** – Multiplayer platform
- Dialog ID system for UI flow control

## 🚀 How to Use

1. Copy all `.inc` files into your `includes/` directory.
2. In your gamemode or filterscript:
   ```pawn
   #include "main"
3.    Bind the trade system to your command processor (e.g., ZCMD or YCMD).
4.    Ensure dialogs and player states are properly initialized on connection.

👨‍💻 Author

Developed by Tigran Kocharov
📧 tiko.nue@icloud.com

📄 License & Attribution

This project was created for a private SA-MP RPG server and is now shared publicly for educational and non-commercial use.
Proper attribution is required if you use or modify this project.
