import java.util.concurrent.*;

public class TrafficController {

    static Semaphore sem;

    public TrafficController()
    {
        sem = new Semaphore(1);
    }

    public void enterLeft() {
        try {
            sem.acquire();
        }
        catch (InterruptedException e) {
            System.out.println("left: " + e);
        }
    }
    public void enterRight() {
        try {
            sem.acquire();
        }
        catch (InterruptedException e) {
            System.out.println("right - " + e);
        }
    }
    public void leaveLeft() {
        sem.release();
    }
    public void leaveRight() {
        sem.release();
    }

}