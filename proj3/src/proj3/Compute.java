package proj3;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Compute extends Remote {
    <T> T executeTask(Task<T> t) throws RemoteException;

    String AnalyzeRequest(String message, int usr_type, Discipline[] college_data) throws RemoteException;
}