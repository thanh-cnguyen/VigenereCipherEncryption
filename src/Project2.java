import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.Semaphore;

public class Project2 {

    static final int MAX_TELLER = 3; // Number of bank tellers
    static final int MAX_CUSTOMER = 100; // Number of customers to serve

    static Queue<Integer> customerLine = new LinkedList<>(); // customerLine stores in-bank Customer IDs
    static Queue<String> transactionLine = new LinkedList<>(); // customerLine stores in-bank Customer IDs
    static Queue<Integer> station = new LinkedList<>(); // Station stores available Teller IDs

    /**
     * @param args
     */
    public static void main(String[] args) {
        try {
            for (int i = 0; i < MAX_TELLER; i++) { // Initialize teller semaphores
                customerApproach[i] = new Semaphore(0);
                askForTransaction[i] = new Semaphore(0);
                waitForTransaction[i] = new Semaphore(0);
                handleTransaction[i] = new Semaphore(0);
                completeTransaction[i] = new Semaphore(0);
                readyToLeave[i] = new Semaphore(0);
            }

            // Create Teller and Customer Threads
            Thread[] tellers = new Thread[MAX_TELLER];
            Thread[] customers = new Thread[MAX_CUSTOMER];

            // Instantiate each thread with a Teller
            for (int i = 0; i < MAX_TELLER; i++) {
                tellers[i] = new Teller(i);
                tellers[i].start();
            }

            // Instantiate each thread with a Customer
            for (int i = 0; i < MAX_CUSTOMER; i++) {
                customers[i] = new Customer(i);
                customers[i].start();
            }

            // Join all Teller before closing the bank
            for (int i = 0; i < MAX_TELLER; i++) {
                tellers[i].join();
            }

            System.out.println("The bank closes for the day.");
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    static Semaphore tellerPrepare = new Semaphore(1); // For teller to let anyone know it is ready
    static Semaphore goToMgn = new Semaphore(1); // For a teller to interact with the manager
    static Semaphore goToSafe = new Semaphore(2); // For 2 tellers to access the safe
    static Semaphore leaveTheBank = new Semaphore(-2); // For all tellers to leave the bank once all customers have left

    static Semaphore[] customerApproach = new Semaphore[3]; // For teller to wait for the Customer.
    static Semaphore[] waitForTransaction = new Semaphore[3]; // For teller to wait for the Customer's transaction.
    static Semaphore[] readyToLeave = new Semaphore[3]; // For teller to leave the bank. tbd

    static boolean tellersAvailability = true; // Flag to let Customers know Tellers are available
    static boolean hasCustomer = true; // Flag to inform Tellers of Customers in line

    public static class Teller extends Thread {

        private int tid; // Unique id of the teller
        private int servingCustomerId; // Current CustomerId

        private static int totalCustomerServed; // Keep track of the number of customers enterring the bank
        private String servingCustomerTransaction; // Current CustomerTransaction
        Random rand = new Random(); // Random number generator

        public Teller(int tid) { // Teller constructor
            this.tid = tid;
        }

        public void tellSim() throws InterruptedException { // Teller Simulation
            int numCustomerServed = 0; // Number of customer this Teller serves
            do {
                tellerPrepare.acquire(); // Tellers get ready
                // Let everyone know it is ready to serve
                System.out.println("Teller " + this.tid + " is ready to serve.");

                // Wait for customer to approach
                System.out.println("Teller " + this.tid + " is waiting for a customer.");
                station.add(this.tid); // Teller available at the station
                if (!tellersAvailability && !station.isEmpty()) {
                    tellersAvailability = true;
                    selectTeller.release();
                }
                tellerPrepare.release(); // Signal Teller already ready
                selectTransaction.release(); // Signal Customers to declare request
                customerApproach[this.tid].acquire(); // WAIT
                servingCustomerId = customerLine.remove();
                System.out.println("Teller " + this.tid + " is serving Customer " + servingCustomerId + ".");
                totalCustomerServed++; // Record amount of customers served per day

                // Ask for transaction
                System.out.println("Teller " + this.tid + " requests transaction: Withdrawal or Deposit");
                askForTransaction[this.tid].release(); // Signal Customer to give their transaction request

                // Wait for customer transaction
                waitForTransaction[this.tid].acquire(); // WAIT
                servingCustomerTransaction = transactionLine.remove(); // Take Customer's transaction request

                // Handle transaction request
                System.out
                        .println("Teller " + this.tid + " is handling the " + servingCustomerTransaction
                                + " transaction.");

                // Perform transaction
                // Teller sees their manager begins
                if (servingCustomerTransaction.equalsIgnoreCase("withdrawal")) {
                    System.out.println("Teller " + this.tid + " is going to the manager.");
                    goToMgn.acquire();
                    System.out.println("Teller " + this.tid + " is getting to the manager's permission.");
                    Thread.sleep((rand.nextInt(30 - 5) + 5));
                    System.out.println("Teller " + this.tid + " got the manager's permission.");
                    goToMgn.release();
                } // Teller sees their manager ends
                System.out.println("Teller " + this.tid + " is going to the safe.");
                // Teller access the safe begins
                goToSafe.acquire();
                System.out.println("Teller " + this.tid + " is in the safe.");
                goToBank.release(); // signal next customer to come in
                Thread.sleep((rand.nextInt(50 - 10) + 10));
                System.out.println("Teller " + this.tid + " is leaving the safe.");
                goToSafe.release();
                // Teller access the safe ends

                // Go back and inform the customer after complete the transaction request
                System.out.println(
                        "Teller " + this.tid + " finishes Customer " + servingCustomerId + "\'s "
                                + servingCustomerTransaction + " transaction.");
                numCustomerServed++; // Record the number of customers served per teller
                completeTransaction[this.tid].release(); // Signal Customer of their complete transaction

                if (totalCustomerServed >= MAX_CUSTOMER - (MAX_TELLER - 1)) {
                    leaveTheBank.release(); // Signal a teller about to leave
                    break;
                }
            } while (hasCustomer); // Tellers keep serving until no customers left at the bank

            // Leave the bank after serving all customers
            readyToLeave[this.tid].acquire(numCustomerServed); // Each Teller waits until their customer leaves
            leaveTheBank.acquire(); // Wait for all customer to finish their work
            System.out.println("Teller " + this.tid + " is leaving for the day.");
            leaveTheBank.release(); // All tellers can leave

        }

        @Override
        public void run() {
            try {
                tellSim(); // Initiate Teller Simulation
            } catch (Exception e) { 
                e.printStackTrace();
                System.exit(1);
            }
        }
    }

    static Semaphore[] handleTransaction = new Semaphore[3]; // For customer to wait for a teller
    static Semaphore[] askForTransaction = new Semaphore[3]; // For customer to wait for a teller
    static Semaphore[] completeTransaction = new Semaphore[3]; // For customer to wait for processing transaction

    static Semaphore goToBank = new Semaphore(0); // For customer to go to a bank
    static Semaphore selectTransaction = new Semaphore(-2); // For customer to declare transaction after all tellers ready
    static Semaphore selectTeller = new Semaphore(0); // For customer to select tellers at the station

    public static class Customer extends Thread {

        private int cid; // Unique id of the teller
        private int currTellerId; // Current in-service Teller
        private String[] transactionTypeOptions = { "withdrawal", "deposit" };
        private String transactionType; // Type of Customer transaction
        private Random rand = new Random(); // Random number generator
        private static int numRequest; // Number of transaction request to be processed

        public Customer(int cid) { // Customer constructor
            this.cid = cid;
            this.transactionType = transactionTypeOptions[rand.nextInt(transactionTypeOptions.length)];
        }

        public void custSim() throws InterruptedException { // Customer Simulation

            // Decide what transaction to perform
            selectTransaction.acquire();
            System.out.println(
                    "Customer " + this.cid + " wants to perform a " + this.transactionType + " transaction.");
            numRequest++; // Capture customer request
            selectTransaction.release();

            // Choose what time of day to go to the bank
            if (numRequest == MAX_CUSTOMER) { // Obtain all customer requests
                goToBank.release();
                numRequest = 0;
            }
            goToBank.acquire();
            customerLine.add(this.cid); // Customer is in waiting customerLine for bank services
            System.out.println("Customer " + this.cid + " is going to the bank.");
            Thread.sleep(rand.nextInt(5000)); // **************************************needs to change back to 5000
            System.out.println("Customer " + this.cid + " enters the bank.");

            // Get in customerLine
            System.out.println("Customer " + this.cid + " is getting in line.");
            System.out.println("Customer " + this.cid + " is selecting a teller.");
            if (station.isEmpty()) {
                tellersAvailability = false;
                selectTeller.acquire();// wait for an available teller
            }
            currTellerId = station.remove(); // Customner select first available Teller
            System.out.println("Customer " + this.cid + " goes to Teller " + currTellerId + ".");

            // Introduce to a teller
            System.out.println("Customer " + this.cid + " introduces itself to Teller " +
                    currTellerId + ".");
            customerApproach[currTellerId].release(); // Signal Teller of Customer incoming

            // Wait for teller to ask for the transaction
            askForTransaction[currTellerId].acquire();

            // Tell the selected transaction to teller
            System.out.println("Customer " + this.cid + " asks for a " + this.transactionType + " transaction.");
            transactionLine.add(this.transactionType); // Customer submit their transaction to be processed
            waitForTransaction[currTellerId].release(); // Signal Teller to check customer transaction

            // Wait for teller to complete the transaction
            completeTransaction[currTellerId].acquire();
            System.out.println("Customer " + this.cid + " thanks Teller " + currTellerId + " and leaves.");
            if (customerLine.isEmpty())
                hasCustomer = false;
            readyToLeave[currTellerId].release(); // Signal Teller may leave
        }

        @Override
        public void run() {
            try {
                custSim(); // Initiate Cusomter Simulation
            } catch (Exception e) {
                e.printStackTrace();
                System.exit(1);
            }
        }
    }
}