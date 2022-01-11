public interface DiningSurface {
    public void waitForSticks(Philosopher phil) throws InterruptedException;
    public void layDownSticks(Philosopher phil);
}