public class Display {
    private StringBuilder line;
    private int turn = 0;

    public Display(int numberOfPhilosophers) {
        if (numberOfPhilosophers < 1){
            numberOfPhilosophers = 1;
        }
        this.line = new StringBuilder("  ".repeat(numberOfPhilosophers - 1) + " ");
    }

    void update(int philosopher, char symbol) {
        synchronized(this) {
            line.setCharAt(philosopher * 2, symbol);
            System.out.printf("%5d: %s\n", this.turn++, this.line.toString());
            System.out.flush();
        }
    }
}