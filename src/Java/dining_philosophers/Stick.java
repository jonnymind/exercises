package dining_philosophers;

class Stick {
    private boolean isUsed = false;
    int id;

    Stick(int id) {
        this.id = id;
    }

    boolean isFree() { return ! isUsed; }
 
    void pickUp() {
        isUsed = true;
    }

    void layDown() {
        isUsed = false;
    }
}