class DiningPhilosophers {
    private static int size = 5;
    private static int duration = 60;
    private static int maxPhilosopherWait = 3;

    public static void main(String args[]) {
        Display disp = new Display(size);
        Table table = new Table(size, disp, maxPhilosopherWait * 1000);
        Thread[] philThreads = new Thread[size];

        System.out.println("Starting...");
        for(int i = 0; i < size; ++i) {
            philThreads[i] = new Thread(table.philosophers[i]);
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