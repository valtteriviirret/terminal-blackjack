# terminal-blackjack
Blackjack game that can be played from Linux terminal

# Gameplay
Regular Blackjack rules: Jack's, Queen's and King's value is 10.
Ace is either 1 or 11 depending on your hand.

You can hit, stand or double any two cards.

Two cards with same value can be splitted just like regular Blackjack.
Oh and there are no house rules. You can split and hit on aces.

The graphics are made with just text. So for example the ace of spades will look about like:
```
----
|♠A|
|  |
----
```

# Compatibility
This game without modifications does not work on Windows, since it uses unistd.h.

# How to run the program:
1. Download files
2. Go to the directory with programs files, and compile with **make**
3. Run the executable **./blackjack**
