package proj3;

public class College {
    public Discipline[] disciplines;
    public int size;

    public College(int size){
        this.disciplines = new Discipline[size];
        for(int i = 0; i < size; i++) {
            disciplines[i] = new Discipline("", "", "", "", "", "");
        }
        this.size = size;
    }
}
