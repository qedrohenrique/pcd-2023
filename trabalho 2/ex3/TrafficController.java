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
            System.out.println("Left Acquire: " +sem.availablePermits());
        }
        catch (InterruptedException e) {
            System.out.println("Left: " + e);
        }
    }
    public void enterRight() {
        try {
            sem.acquire();
            System.out.println("Right Acquire: " +sem.availablePermits());
        }
        catch (InterruptedException e) {
            System.out.println("Right - " + e);
        }
    }
    public void leaveLeft() {
        sem.release();
        System.out.println("Left Released: " +sem.availablePermits());
    }
    public void leaveRight() {
        sem.release();
        System.out.println("Right released: " +sem.availablePermits());
    }

}