// package proj3;

//import java.io.*;
//import java.nio.file.*;

public class ConnectionTime {
    public int operation;
    public long sendTime;
    public long receiveTime;

    public ConnectionTime(int op, long send, long rcv){
        this.operation = op;
        this.sendTime = send;
        this.receiveTime = rcv;
    }

    public void printConnectionTimeClient() {
        System.out.println(">>>>>Send Time: " + this.sendTime/1000 + " μs");
        System.out.println(">>>>>Receive Time: " + this.receiveTime/1000 + " μs");
        System.out.println(">>>>>Total Interval Time: " + (this.sendTime/1000 - this.receiveTime/1000) + " μs");

//        writeClientTimeResults("ClientTimeLog.txt");
    }

    public void printExecutionTimeServer() {
        System.out.println(">>>>>Receive Time: " + this.receiveTime/1000 + " μs");
        System.out.println(">>>>>Send Time: " + this.sendTime/1000 + " μs");
        System.out.println(">>>>>Total Interval Time: " + (this.sendTime/1000 - this.receiveTime/1000) + " μs");

//        writeServerTimeResults("ServerTimeLog.txt");
    }

//    public void writeClientTimeResults(String filename) {
//
//    }
//
//    public void writeServerTimeResults(String filename) {
//
//    }


}
