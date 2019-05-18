package OOP.Solution;

import OOP.Provided.CasaDeBurrito;
import OOP.Provided.Profesor;

import java.util.*;
import java.util.stream.Collectors;

public class CasaDeBurritoImpl implements CasaDeBurrito {
    private int id;
    private String name;
    private int distance;
    private HashMap<Profesor, Integer> map;
    private Set<String> menu;

    public CasaDeBurritoImpl(int id, String name, int dist, Set<String> menu) {
        this.id = id;
        this.name = name;
        this.distance = dist;
        this.map = new HashMap<>();
        // deep copy the menu isn't needed by pdf
        this.menu = (menu == null) ? new HashSet<>() : new HashSet<>(menu);
    }

    public int getId() {
        return id;
    }

    public String getName() {return name; }

    public int distance() {
        return distance;
    }

    public boolean isRatedBy(Profesor p) {
        if (p == null) return false;
        return map.containsKey(p);
    }

    public CasaDeBurrito rate(Profesor p, int r) throws RateRangeException {
        if (p == null) return this;
        // if rated before, then rated illegally , leaves old rank
        if (r < 0 || r > 5) throw new RateRangeException();
        map.put(p, r);
        return this;
    }

    public int numberOfRates() {
        return map.size();
    }

    public double averageRating() {
        if (map.size() == 0) return 0;
        int sum = 0;
        for (int rate : map.values()) {
            sum += rate;
        }
        return sum / (double)map.size();
    }

    public boolean equals(Object o) {
        if (o == null) return false;
        if (o.getClass() != this.getClass()) return false;
        return id == ((CasaDeBurritoImpl)o).getId();
    }

    public String toString() {
        String res = "CasaDeBurrito: " + name + ".\nId: " + id;
        res += ".\nDistance: " + distance + ".\nMenu: ";
        res += menu.stream().sorted().collect(Collectors.joining(", "));
        res +=".\n";
        return res;
    }

    public int compareTo(CasaDeBurrito c) {
        assert(c != null);
        return this.id - c.getId();
    }

    public int hashCode() {
        return this.id;
    }

}
