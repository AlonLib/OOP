package OOP.Solution;

import OOP.Provided.CasaDeBurrito;
import OOP.Provided.Profesor;

import java.util.*;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class ProfesorImpl implements Profesor {
    private int id;
    private String name;
    private Set<Profesor> friends;
    private Set<CasaDeBurrito> favorites;

    public ProfesorImpl(int id, String name) {
        this.id = id;
        this.name = name;
        this.friends = new HashSet<>();
        this.favorites = new HashSet<>();
    }

    public int getId() {
        return id;
    }

    public Profesor favorite(CasaDeBurrito c)
            throws UnratedFavoriteCasaDeBurritoException {
        if (c != null && c.isRatedBy(this)) {
            favorites.add(c);
            return this;
        }
        throw new UnratedFavoriteCasaDeBurritoException();
    }

    public Collection<CasaDeBurrito> favorites() {
        return (new HashSet<>(favorites))
                .stream()
                .collect(Collectors.toSet());
    }

    public Profesor addFriend(Profesor p)
            throws SameProfesorException, ConnectionAlreadyExistsException {
        if (p == null) return this;
        if (this.equals(p)) throw new SameProfesorException();
        if (friends.add(p)) return this;
        // Set add returns bool: if the new element existed in the set before.
        throw new ConnectionAlreadyExistsException();
    }

    public Set<Profesor> getFriends() {
        return (new HashSet<>(friends))
                .stream()
                .collect(Collectors.toSet());
    }

    public Set<Profesor> filteredFriends(Predicate<Profesor> p) {
        return (new HashSet<>(friends))
                .stream()
                .filter(p)
                .collect(Collectors.toSet());
    }

    public Collection<CasaDeBurrito> filterAndSortFavorites
            (Comparator<CasaDeBurrito> comp, Predicate<CasaDeBurrito> p) {
        // Changes in returned collection may affect the original collection.
        return (new LinkedList<>(favorites))
                .stream()
                .sorted(comp)
                .filter(p)
                .collect(Collectors.toList());
    }

    private Comparator<CasaDeBurrito> byRate =
            Comparator.comparingDouble(CasaDeBurrito::averageRating);
    private Comparator<CasaDeBurrito> byDistance =
            Comparator.comparingInt(CasaDeBurrito::distance);
    private Comparator<CasaDeBurrito> byId =
            Comparator.comparingInt(CasaDeBurrito::getId);

    public Collection<CasaDeBurrito> favoritesByRating(int rLimit) {
        return this
                .filterAndSortFavorites(
                        byRate.reversed()
                                .thenComparing(byDistance)
                                .thenComparing(byId),
                        (c) -> c.averageRating() >= rLimit);
    }

    public Collection<CasaDeBurrito> favoritesByDist(int dLimit) {
        return this
                .filterAndSortFavorites(
                        byDistance
                                .thenComparing(byRate.reversed())
                                .thenComparing(byId),
                        (c) -> c.distance() <= dLimit);
    }

    public boolean equals(Object o) {
        if (o == null) return false;
        if (o.getClass() != this.getClass()) return false;
        return id == ((Profesor) o).getId();
    }

    public String toString() {
        String res = "Profesor: " + name + ".\nId: " + id + ".\nFavorites: ";
        res += favorites
                .stream()
                .map(CasaDeBurrito::getName)
                .sorted()
                .collect(Collectors.joining(", "));
        res += ".\n";
        return res;
    }

    public int compareTo(Profesor p) {
        assert (p != null);
        return this.id - p.getId();
    }

    public int hashCode() {
        return this.id;
    }

}



