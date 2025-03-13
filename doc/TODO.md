Quick:

- Check if movegen is right
- Try to render more than one move
- Implement unmake move

Move Generation:

- Generate castling move
- Add make and unmake move ability
- Generate legal moves from psuedo legal moves
- Connect moves array to UCI [LATER]

- Generate sliding piece moves with magic bitboards
- Implement En Passant move


Searching:

- Alpha-Beta
- Prune Branches
- Move Order (Promotion & Capture Moves First)

Evaluating:
- When Endgame: King Should Be Closer To King (If Winning)
- When Endgame: Opponent King Should Be Closer To Edges (If Winning)
- Transposition Tables via Zobris Hashing
