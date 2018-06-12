package proj3;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Compute extends Remote {
    String analyzeRequest(String message) throws RemoteException;
}
