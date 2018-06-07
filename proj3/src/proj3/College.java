package proj3;

public class College {
    public Discipline[] disciplines;
    public int size;

    public College(int size){
        this.disciplines = new Discipline[size];
        this.size = size;
    }
}
