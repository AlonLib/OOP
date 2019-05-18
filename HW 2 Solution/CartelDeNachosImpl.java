package OOP.Solution;

import OOP.Provided.CartelDeNachos;
import OOP.Provided.CasaDeBurrito;
import OOP.Provided.Profesor;

import java.util.*;
import java.util.stream.Collector;
import java.util.stream.Collectors;

public class CartelDeNachosImpl implements CartelDeNachos {

    private Set<Profesor> members;
    private Set<CasaDeBurrito> casas;

    public CartelDeNachosImpl() {
        this.members = new HashSet<>();
        this.casas = new HashSet<>();
    }

    public Profesor joinCartel(int id, String name)
            throws Profesor.ProfesorAlreadyInSystemException {
        Profesor p = new ProfesorImpl(id, name);
        if (members.add(p))
            return p;
        // if returns true if members already contains p.
        throw new Profesor.ProfesorAlreadyInSystemException();
    }

    public CasaDeBurrito addCasaDeBurrito
            (int id, String name, int dist, Set<String> menu)
            throws CasaDeBurrito.CasaDeBurritoAlreadyInSystemException {
        CasaDeBurrito c = new CasaDeBurritoImpl(id, name, dist, menu);
        if (casas.add(c))
            return c;
        // if returns true if casas already contains c.
        throw new CasaDeBurrito.CasaDeBurritoAlreadyInSystemException();
    }

    public Collection<Profesor> registeredProfesores() {
        return (new HashSet<>(members))
                .stream()
                .collect(Collectors.toSet());
    }

    public Collection<CasaDeBurrito> registeredCasasDeBurrito() {
        return (new HashSet<>(casas))
                .stream()
                .collect(Collectors.toSet());

    }

    public Profesor getProfesor(int id)
            throws Profesor.ProfesorNotInSystemException {
        for (Profesor p : members) {
            if (p.getId() == id)
                return p;
        }
        throw new Profesor.ProfesorNotInSystemException();
    }

    public CasaDeBurrito getCasaDeBurrito(int id)
            throws CasaDeBurrito.CasaDeBurritoNotInSystemException {
        for (CasaDeBurrito c : casas) {
            if (c.getId() == id)
                return c;
        }
        throw new CasaDeBurrito.CasaDeBurritoNotInSystemException();
    }

    public CartelDeNachos addConnection(Profesor p1, Profesor p2)
            throws Profesor.ProfesorNotInSystemException,
            Profesor.ConnectionAlreadyExistsException,
            Profesor.SameProfesorException {
        assert (p1 != null && p2 != null);
        if (!members.contains(p1) || !members.contains(p2))
            throw new Profesor.ProfesorNotInSystemException();
        // one-sided friendly connection is not possible by the pdf
        p1.addFriend(p2);
        p2.addFriend(p1);
        return this;
    }

    // fix duplicated code here?
    public Collection<CasaDeBurrito> favoritesByRating(Profesor p)
            throws Profesor.ProfesorNotInSystemException {
        assert (p != null);
        if (!members.contains(p))
            throw new Profesor.ProfesorNotInSystemException();
        Set<Profesor> friends = p.getFriends();
        List<CasaDeBurrito> res = new LinkedList<>();
        for (Profesor f : friends.stream().sorted().collect(Collectors.toList())) {
            res.addAll(f.favoritesByRating(-1));
        }
        return res.stream().distinct().collect(Collectors.toList());
    }

    public Collection<CasaDeBurrito> favoritesByDist(Profesor p)
            throws Profesor.ProfesorNotInSystemException {
        assert (p != null);
        if (!members.contains(p))
            throw new Profesor.ProfesorNotInSystemException();
        Set<Profesor> friends = p.getFriends();
        List<CasaDeBurrito> res = new LinkedList<>();
        for (Profesor f : friends.stream().sorted().collect(Collectors.toList())) {
            res.addAll(f.favoritesByDist(Integer.MAX_VALUE));
        }
        return res.stream().distinct().collect(Collectors.toList());
    }

    private boolean getRecommendationRecursive
            (Profesor p, CasaDeBurrito c, int t) {
        assert (p != null && c != null);
        if (t <= -1) return false;
        for (CasaDeBurrito casa : p.favorites()) {
            if (casa.equals(c)) return true;
        }
        if (t == 0) return false;
        for (Profesor f : p.getFriends()) {
            if (getRecommendationRecursive(f, c, t - 1)) return true;
        }
        return false;
    }

    public boolean getRecommendation(Profesor p, CasaDeBurrito c, int t)
            throws Profesor.ProfesorNotInSystemException,
            CasaDeBurrito.CasaDeBurritoNotInSystemException,
            CartelDeNachos.ImpossibleConnectionException {
        assert (p != null && c != null);
        if (!members.contains(p))
            throw new Profesor.ProfesorNotInSystemException();
        if (!casas.contains(c))
            throw new CasaDeBurrito.CasaDeBurritoNotInSystemException();
        if (t < 0)
            throw new CartelDeNachos.ImpossibleConnectionException();

        return getRecommendationRecursive(p, c, t);
    }

    public List<Integer> getMostPopularRestaurantsIds() {
        Map<CasaDeBurrito, Integer> rank = new HashMap<>();
        for (Profesor p : members) {
            for (Profesor f : p.getFriends()) {
                for (CasaDeBurrito c : f.favorites()) {
                    if (rank.get(c) == null) {
                        rank.put(c, 1);
                    } else {
                        rank.replace(c, rank.get(c) + 1);
                    }
                }
            }
        }

        if (rank.isEmpty())
            return casas
                    .stream()
                    .map(CasaDeBurrito::getId)
                    .collect(Collectors.toList());

        int max = Collections.max(rank.entrySet(),
                Map.Entry.comparingByValue()).getValue();

        List<Map.Entry<CasaDeBurrito, Integer>> score =
                new LinkedList<>(rank.entrySet());

        return score
                .stream()
                .filter(a -> a.getValue() == max)
                .map(a -> a.getKey().getId())
                .collect(Collectors.toList());
    }

    public String toString() {
        String res = "Registered profesores: ";
        res += members
                .stream()
                .map(Profesor::getId)
                .sorted()
                .map(a -> a.toString())
                .collect(Collectors.joining(", "));
        res += ".\nRegistered casas de burrito: ";
        res += casas
                .stream()
                .map(CasaDeBurrito::getId)
                .sorted()
                .map(a -> a.toString())
                .collect(Collectors.joining(", "));
        res += ".\nProfesores:\n";
        for (Profesor p : members) {
            res += p.getId() + " -> [";
            res += p.getFriends()
                    .stream()
                    .map(Profesor::getId)
                    .sorted()
                    .map(a -> a.toString())
                    .collect(Collectors.joining(", "));
            res += "].\n";
        }
        res += "End profesores.\n";
        return res;
    }

}
