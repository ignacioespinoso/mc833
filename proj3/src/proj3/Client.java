package proj3;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Client {
    public String request;

    public static void main(String args[]) {
        // if (System.getSecurityManager() == null) {
        //     System.setSecurityManager(new SecurityManager());
        // }
        try {
            Client client = new Client();
            ConnectionTime op = new ConnectionTime(-1, -1, -1);
            String name = "Compute";
            Registry registry = LocateRegistry.getRegistry(args[0]);
            Compute comp = (Compute) registry.lookup(name);

            client.checkTestMode(args, comp);

            while(client.selectRequestMessage(op)) {
                op.sendTime = System.nanoTime();
                String answer = comp.analyzeRequest(client.request);
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            }

        } catch (Exception e) {
            System.err.println("Client exception:");
            e.printStackTrace();
        }
    }


    public boolean selectRequestMessage(ConnectionTime op) {
        int option;

        System.out.println("Make a request or send a message:");
        System.out.println("1 - Get all subjects");
        System.out.println("2 - Get subject description");
        System.out.println("3 - Get subject full information");
        System.out.println("4 - Get next class information");
        System.out.println("5 - Get all subjects info");

//        Teacher only
        System.out.println("6 - Set nextClassComment");
        System.out.println("7 - Send custom message");

        System.out.println("0 - Close Connection");

//        Get user command
        Scanner scanner = new Scanner(System.in);
        option = scanner.nextInt();

//      Time counter gets operation.
        op.operation = option;

        scanner.nextLine();
//        Save the request on the time operation
      //  op.operation = option;

        switch (option) {
            case 0:
                // Close connection
                return false;
            case 1:
                request = "1 Get all subjects";
                return true;
            case 2:
                request = "2 ";
                break;
            case 3:
                request = "3 ";
                break;
            case 4:
                request = "4 ";
                break;
            case 5:
                request = "5 Get all subjects info";
                return true;
            case 6:
                request = "6 ";
                String change;

                // Get the subject
                System.out.print("Type the subject you want to change next class commentary: ");
                change = scanner.nextLine();
                request += change + " ";

                // Get the next class comment
                System.out.println("Type the new Commentary: ");
                request += scanner.nextLine();

                System.out.println("MESSAGE: " + request);
                return true;
            case 7:
                System.out.print("Type your message: ");
                request = "6 ";
                return true;
            case 9:
                System.out.println("Logging as a teacher...");
                request = "9 ";
                return true;
            default:
                System.out.println("Invalid request... Closing connection");
                return false;
        }

        String code;
        System.out.println("For which Subject [Type subject Code]: ");
        code = scanner.nextLine();
        request += code;

        return true;
    }
//    Check if the user run the program on TEST MODE
    public void checkTestMode(String[] args, Compute comp) {
        if((args.length > 1) && (args[1].equals("TEST"))) {
            executeTestMode(comp);
        }
    }

//    This method is used on TEST MODE
//    to make 50 connections of all types to get the current
//    execution and connection time
    public void executeTestMode(Compute comp) {
        String answer;
        ConnectionTime op = new ConnectionTime(-1, -1, -1);
        System.out.println("-------- TEST MODE ---------\n");

        System.out.println(">>>>>>> Category 1 Messages:\nGet all subjects and names\n");
        op.operation = 1;
        for(int i = 0; i < 50; i++) {
            try {
                op.sendTime = System.nanoTime();
                answer = comp.analyzeRequest("1 Get all subjects");
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            } catch (Exception e) {
            System.err.println("Client exception:");
                e.printStackTrace();
            }
        }

        System.out.println(">>>>>>> Category 2 Messages:\nGet subject description\n");
        op.operation = 2;
        for(int i = 0; i < 50; i++) {
            try {
                op.sendTime = System.nanoTime();
                answer = comp.analyzeRequest("2 MC102");
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            } catch (Exception e) {
                System.err.println("Client exception:");
                e.printStackTrace();
            }
        }

        System.out.println(">>>>>>> Category 3 Messages:\nGet subject full information\n");
        op.operation = 3;
        for(int i = 0; i < 50; i++) {
            try {
                op.sendTime = System.nanoTime();
                answer = comp.analyzeRequest("3 MA111");
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            } catch (Exception e) {
                System.err.println("Client exception:");
                e.printStackTrace();
            }
        }

        System.out.println(">>>>>>> Category 4 Messages:\nGet next class information\n");
        op.operation = 4;
        for(int i = 0; i < 50; i++) {
            try {
                op.sendTime = System.nanoTime();
                answer = comp.analyzeRequest("4 EE532");
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            } catch (Exception e) {
                System.err.println("Client exception:");
                e.printStackTrace();
            }
        }

        System.out.println(">>>>>>> Category 5 Messages:\nGet all Subjects info\n");
        op.operation = 5;
        for(int i = 0; i < 50; i++) {
            try {
                op.sendTime = System.nanoTime();
                answer = comp.analyzeRequest("5 Get all subjects info");
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            } catch (Exception e) {
                System.err.println("Client exception:");
                e.printStackTrace();
            }
        }

        System.out.println(">>>>>>> Category 6 Messages:\nWrite new Next Class information\n");
        op.operation = 6;
        for(int i = 0; i < 50; i++) {
            try {
                op.sendTime = System.nanoTime();
                answer = comp.analyzeRequest("6 EE532 Trocando comentario por este daqui");
                op.receiveTime = System.nanoTime();
                System.out.println(answer);
                op.printConnectionTimeClient();
            } catch (Exception e) {
                System.err.println("Client exception:");
                e.printStackTrace();
            }
        }

        System.out.print("\n\n-------- TEST MODE FINISHED ---------\n");
        System.out.print("check the Log file to get the system time easily\n\n");

        System.exit(0);
    }

}
