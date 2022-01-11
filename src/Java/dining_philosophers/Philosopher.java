package dining_philosophers;

import java.util.concurrent.ThreadLocalRandom;

public class Philosopher implements Runnable {
    Stick leftStick;
    Stick rightStick;
    private Display display;
    int id;
    private int maxWait;
    private boolean end = false;
    private Table table;

    public Philosopher(int id, Table table, Display disp, int maxWait) {
        this.id = id;
        var sticks = table.seat(id);
        this.leftStick = sticks[0];
        this.rightStick = sticks[1];
        this.display = disp;
        this.maxWait = maxWait;
        this.table = table;
    }

    private void think() throws InterruptedException{
        display.update(id, PhilosopherStauts.THINKING);
        Thread.sleep(ThreadLocalRandom.current().nextLong(100, maxWait));
    }

    private void eat() throws InterruptedException {
        display.update(id, PhilosopherStauts.EATING);
        Thread.sleep(ThreadLocalRandom.current().nextLong(100, maxWait));
    }

    public void run() {
        while (true) {
            try {
                think();
                table.waitForSticks(this);
                eat();
                table.layDown(leftStick, rightStick);
            }
            catch(InterruptedException e) {
                display.update(id, PhilosopherStauts.TERMINATING);
                break;
            }
        }
    }

    void displayWait() {
        display.update(id, PhilosopherStauts.WAITING);
    }
}