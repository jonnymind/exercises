package dining_philosophers;

public class Display {
    private StringBuilder line;
    private int turn = 0;

    public Display(int numberOfProphets) {
        if (numberOfProphets < 1){
            numberOfProphets = 1;
        }
        this.line = new StringBuilder("  ".repeat(numberOfProphets - 1) + " ");
    }

    void update(int philosopher, char symbol s) {
        synchronized(this) {
            line.setCharAt(philosopher * 2, symbol);
            System.out.printf("%5d: %s\n", this.turn++, this.line.toString());
            System.out.flush();
        }
    }
}