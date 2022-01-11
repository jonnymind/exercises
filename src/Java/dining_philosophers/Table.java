package dining_philosophers;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Table {

    Stick[] sticks;
    Lock mutex = new ReentrantLock();
    Condition sticksChanged = mutex.newCondition();
    List<Philosopher> waitingList = new LinkedList<Philosopher>();

    public Table(int stickCount) {
        this.sticks = new Stick[stickCount];
        for(int i = 0; i < stickCount; ++i) {
            this.sticks[i] = new Stick(i);
        }
    }
    
    Stick[] seat(int philosopherId) {
        Stick[] pair = new Stick[2];
        pair[0] = sticks[philosopherId];
        philosopherId++;
        if(philosopherId >= sticks.length) {
            philosopherId = 0;
        }
        pair[1] = sticks[philosopherId];

        return pair;
    }

    void waitForSticks(Philosopher phil) throws InterruptedException {
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

    void layDown(Stick left, Stick right) {
        mutex.lock();
        left.layDown();
        right.layDown();
        sticksChanged.signalAll();
        mutex.unlock();
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
