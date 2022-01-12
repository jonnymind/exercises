import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Table implements DiningSurface {

    Stick[] sticks;
    Philosopher[] philosophers;
    Lock mutex = new ReentrantLock();
    Condition sticksChanged = mutex.newCondition();
    List<Philosopher> waitingList = new LinkedList<Philosopher>();

    public Table(int philosopherCount, Display display, int maxWait) {
        this.philosophers = new Philosopher[philosopherCount];
        this.sticks = new Stick[philosopherCount];
        for(int i = 0; i < philosopherCount; ++i) {
            this.sticks[i] = new Stick(i);
            this.philosophers[i] = new Philosopher(i, this, display, maxWait);
        }

        for(int i = 0; i < philosopherCount; ++i) {
            seatPhilosophers(philosophers[i]);
        }
    }

    @Override
    public void waitForSticks(Philosopher phil) throws InterruptedException {
        Stick left = phil.leftStick;
        Stick right = phil.rightStick;
        mutex.lock();
        waitingList.add(phil);
        try {
            while(!canProgress(phil)) {
                phil.displayWait();
                sticksChanged.await();
            }
            waitingList.remove(phil);
            sticksChanged.signalAll();

            left.pickUp();
            right.pickUp();
        }
        catch(InterruptedException e) {
            throw e;
        }
        finally {
            mutex.unlock();
        }   
    }

    @Override
    public void layDownSticks(Philosopher phil) {
        mutex.lock();
        phil.leftStick.layDown();
        phil.rightStick.layDown();
        sticksChanged.signalAll();
        mutex.unlock();
    }

    private void seatPhilosophers(Philosopher phil) {
        int id = phil.id;
        phil.leftStick = sticks[id];
        id++;
        if(id >= sticks.length) {
            id = 0;
        }
        phil.rightStick = sticks[id];
    }

    private boolean canProgress(Philosopher phil) {
        return phil.leftStick.isFree() && phil.rightStick.isFree()
            && (phil == waitingList.get(0) 
                || waitingList.stream().filter(wphil -> adjacent(wphil, phil)).count() == 2);
    }

    private boolean adjacent(Philosopher left, Philosopher right) {
        return left.id == right.id + 1 || left.id == right.id -1 
            || (left.id == 0 && right.id == sticks.length - 1)
            || (left.id == sticks.length - 1 && right.id == 0);
    }
}
