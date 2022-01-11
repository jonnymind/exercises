import java.util.concurrent.ThreadLocalRandom;

public class Philosopher implements Runnable {
    int id;
    Stick leftStick;
    Stick rightStick;
    
    private int maxWait;
    private Display display;
    private DiningSurface table;

    public Philosopher(int id, DiningSurface table, Display disp, int maxWait) {
        this.id = id;
        this.table = table;
        this.display = disp;
        this.maxWait = maxWait;
    }

    private void think() throws InterruptedException{
        display.update(id, 'T');
        Thread.sleep(ThreadLocalRandom.current().nextLong(100, maxWait));
    }

    private void eat() throws InterruptedException {
        display.update(id, 'E');
        Thread.sleep(ThreadLocalRandom.current().nextLong(100, maxWait));
    }

    public void run() {
        while (true) {
            try {
                think();
                table.waitForSticks(this);
                eat();
                table.layDownSticks(this);
            }
            catch(InterruptedException e) {
                display.update(id, 'x');
                break;
            }
        }
    }

    void displayWait() {
        display.update(id, 'W');
    }
}