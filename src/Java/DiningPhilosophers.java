import dining_philosophers.*;

class DiningPhilosophers {
    private static int size = 5;
    private static int duration = 60;

    public static void main(String args[]) {
        Table table = new Table(size);
        Philosopher[] phils = new Philosopher[size];
        Display disp =  new Display(size);

        for(int i = 0; i < size; ++i) {
            phils[i] = new Philosopher(i, table, disp, 3000);
        }

        Thread[] philThreads = new Thread[size];

        System.out.println("Starting...");
        for(int i = 0; i < size; ++i) {
            philThreads[i] = new Thread(phils[i]);
            philThreads[i].start();
        }

        try {
            Thread.sleep(duration*1000);
            System.out.println("Terminating...");

            for(int i = 0; i < size; ++i) {
                philThreads[i].interrupt();
            }

            for(int i = 0; i < size; ++i) {
                philThreads[i].join();
            }
        }
        catch(InterruptedException e) {
        }
    }    
}