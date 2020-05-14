import java.util.HashSet;
import java.util.Random;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

public class SantaClausProblem {

    private int elfCount;
    private int reindeerCount;

    private Semaphore santaSem;
    private Semaphore reindeerSem;
    private Semaphore elfSem;

    private Semaphore counterMutex;
    private Semaphore elfMutex;

    private volatile boolean endOfTheWorld = false;
    private final Semaphore stopSem = new Semaphore(0);
    private int stopCounter = 10;
    private Random generator = new Random();

    private int NUM_ELVES_IN_GROUP = 3;
    private int NUM_REINDEERS_IN_GROUP = 9;

    public void init() {

        // Shared data

        elfCount = 0;
        reindeerCount = 0;

        santaSem = new Semaphore(0);
        reindeerSem = new Semaphore(0, true);
        elfSem = new Semaphore(0, true);

        counterMutex = new Semaphore(1, true);
        elfMutex = new Semaphore(1, true);
    }

    class Reindeer implements Runnable {
        int id;

        public Reindeer(int id) {
            this.id = id;
        }

        public void run() {
            while (!endOfTheWorld) {
                try {
                    Thread.sleep(generator.nextInt(300));
                    // protect the counters
                    counterMutex.acquire();
                    System.out.printf("Reindeer %d arrived (%d)\n", id, reindeerCount+1);
                    // increment no of reindeers
                    reindeerCount++;
                    if (reindeerCount == NUM_REINDEERS_IN_GROUP) {
                        // stop if end of the world
                        stopCounter--;
                        if (stopCounter == 0) {
                            endOfTheWorld = true;
                            stopSem.release();
                        }
                        // wake up santa
                        santaSem.release();
                    }
                    counterMutex.release();

                    // wait in reindeer queue
                    reindeerSem.acquire();

                    // get hitched to the sleigh
                    getHitched();
                } catch (InterruptedException ignored) {
                }
            }
            System.out.printf("Reindeer %d is fading away\n", id);
        }

        private void getHitched() {
            System.out.printf("Reindeer %d is getting hitched\n", id);
            try {
                Thread.sleep(generator.nextInt(300));
            } catch (InterruptedException ignored) {
            }
        }
    }

    class Elf implements Runnable {
        private int id;

        public Elf(int id) {
            this.id = id;
        }

        public void run() {

            try {
                Thread.sleep(generator.nextInt(500));

                while (!endOfTheWorld) {
                    elfMutex.acquire();
                    System.out.printf("Elf %d entered (%d)\n", id, elfCount+1);
                    counterMutex.acquire();
                    elfCount++;
                    if (elfCount == NUM_ELVES_IN_GROUP) {
                        // wake up santa, keep the elf mutex on
                        // so other elves cannot enter
                        // while the current group of
                        // elves is getting help from santa
                        santaSem.release();
                    } else {
                        // signal other elves that are waiting
                        // so that they can join the queue
                        elfMutex.release();
                    }
                    counterMutex.release();
                    // wait until a group of elves is complete
                    elfSem.acquire();
                    // get help from santa
                    getHelp();
                    counterMutex.acquire();
                    // decrement elf count
                    elfCount--;
                    if (elfCount == 0) {
                        // after last elf exits, release elf mutex
                        // so other elves can join
                        elfMutex.release();
                    }
                    counterMutex.release();
                }
            } catch (InterruptedException e) {
            }
            System.out.printf("Elf %d is fading away\n", id);

        }

        private void getHelp() {
            System.out.printf("Elf %d is getting help\n", id);
            try {
                Thread.sleep(generator.nextInt(400));
            } catch (InterruptedException ignored) {
            }
        }
    }

    class Santa implements Runnable {

        public void run() {
            while (!endOfTheWorld) {
                try {
                    // wait until a group of elves or reindeers are ready
                    santaSem.acquire();
                    // protect counters
                    counterMutex.acquire();
                    System.out.printf("\nSanta wakes up\n");
                    if (reindeerCount == NUM_REINDEERS_IN_GROUP) {
                        // update reindeer count
                        reindeerCount = 0;
                        // prep sleigh so that reindeers can get hitched
                        prepSleigh();
                        // wake up all reindeers that are waiting for Santa
                        reindeerSem.release(NUM_REINDEERS_IN_GROUP);
                    }
                    else if (elfCount == NUM_ELVES_IN_GROUP) {
                        // help the group of elves
                        helpElves();
                        // wake up the elves
                        elfSem.release(NUM_ELVES_IN_GROUP);
                    }
                    counterMutex.release();
                } catch (InterruptedException ignored) {
                }
            }
            System.out.println("Santa is fading away");
        }


        public void prepSleigh() {
            System.out.print("Santa is prepping the sleigh\n");
            try {
                Thread.sleep(700);
            } catch (InterruptedException ignored) {
            }
        }

        public void helpElves() {
            System.out.print("Santa is helping the elves\n");
            try {
                Thread.sleep(1100);
            } catch (InterruptedException ignored) {
            }
        }
    }

    public SantaClausProblem() {
        try {
            HashSet<Thread> threads = new HashSet<>();
            threads.add(new Thread(new Santa()));

            int numOfElves = 10;
            int numOfReindeers = 9;

            for (int i = 0; i < numOfElves; i++) {
                threads.add(new Thread(new Elf(i)));
            }
            for (int i = 0; i < numOfReindeers; i++) {
                threads.add(new Thread(new Reindeer(i)));
            }

            init();

            for (Thread t : threads) {
                t.start();
            }

            try {
                // wait until end of the world
                stopSem.acquire();
                System.out.println("THE END HAS COME!");
                for (Thread t : threads)
                    t.interrupt();
                for (Thread t : threads)
                    t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Fin");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new SantaClausProblem();
    }
}