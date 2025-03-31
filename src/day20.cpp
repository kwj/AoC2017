/*
 tick  particle (pos, vel, acc)
------------------
    0  (p, v, a)
    1  (p+v+a, v+a, a)
    2  (p+v+a+v+a+a, v+a+a, a) = (p+2v+3a, v+2a, a)
    3  (p+2v+3a+v+2a+a, v+2a+a, a) = (p+3v+6a, v+3a, a)
    4  (p+3v+6a+v+3a+a, v+3a+a, a) = (p+4v+10a, v+4a, a)
     ...
    k  (p+k*v+(k(k+1)/2)*a, v+k*a, a)

  pos(k) = p + k*v + (k/(k+1)/2)*a
         = p + k*v + (k^2 * (a/2) +  k * a/2)
         = p + k * (v + a/2) + k^2 * a/2

[Part 1]
Assuming a situation where an infinite amount of time has elapsed, the one with
the smallest magnitudo of acceleration is the answer we seek. However, we can't
fix the answer by this method when multiple particles with same magnitude of
acceleration exist.

When the magnitudes of acceleration are equal, the smaller magnitude of initial
velocity is not necessarily closer to the origin in the long term. For example,
it could be said that the particle/2 is closer to the origin than the particle/1
after a sufficient time has elapsed.

  particle/1: p=< 0,0,0>, v=< 0,0,0>, a=<1,0,0>
  particle/2: p=< 0,0,0>, v=<-1,0,0>, a=<1,0,0>

Even if magnitudes of acceleration and velocity are equal, particle whose
the initial position closer to the origin is not necessarily closer to the
origin in the long term. Similarly, it could be said that the particle/4 is
closer to the origin than the particle/3 in the following example.

  particle/3: p=< 0,0,0>, v=< 1,0,0>, a=<0,0,0>
  particle/4: p=<-1,0,0>, v=< 1,0,0>, a=<0,0,0>

In other words, neither initial velocity nor initial position can be used as
a tie-breaker.


There were three particles with the smallest magnitude of acceleration in my
given data. I thought of two ways to solve the problem.

One was to determine the answer by positions of particles after a time that
seems sufficient. The other was to determine the answer by to measure distances
at multiple times and checks for changes. Since neither method have basis,
I chose the latter, which seems more tedious in this case.

The distances were measured several times at 1000 tick intervals, and I determined
the closest particle to the origin when there were no changes in the order between
particles and the distance traveled increased monotonically.

A conceptual diagram for the case of three particles and three times of checking
is shown below.

  Dist_p0(t1)    <=    Dist_p1(t1)    <=    Dist_p2(t1)
       |                    |                    |
       |                    |                    |
  Δ_p0(t1,t2)    <=    Δ_p1(t1,t2)    <=    Δ_p2(t1,t2)
       |                    |                    |
       v                    v                    v
  Dist_p0(t2)    <=    Dist_p1(t2)    <=    Dist_p2(t2)
       |                    |                    |
       |                    |                    |
  Δ_p0(t2,t3)    <=    Δ_p1(t2,t3)    <=    Δ_p2(t2,t3)
       |                    |                    |
       v                    v                    v
  Dist_p0(t3)    <=    Dist_p1(t3)    <=    Dist_p2(t3)

    (*) Δ_p#(j,k) = Dist_p#(k) - Dist_p#(j)

and then,

  Δ_p0(t1,t2) <= Δ_p0(t2,t3)
  Δ_p1(t1,t2) <= Δ_p1(t2,t3)
  Δ_p2(t1,t2) <= Δ_p2(t2,t3)

[Part 2]
Examine up the particles and times of possible collisions and check them all
in time order.

If two particles collide at time T:
  particle/1 (p1, v1, a1)
  particle/2 (p2, v2, a2)

  2*p1 + T * (2*v1 + a1) + T^2 * a1 = 2*p2 + T * (2*v2 + a2) + T^2 * a2
 -->
  (a1 - a2) * T^2 + (2*v1 + a1 - 2*v2 - a2) * T + 2 * (p1 -p2) = 0
  ^^^(A)^^^         ^^^^^^^^^^^(B)^^^^^^^^^       ^^^^^(C)^^^^

  discriminant D = B^2 - 4AC
*/

module;

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <istream>
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

import util;

// --------
export module day20;

export namespace day20 {

class [[nodiscard]] Particle {
  public:
    long id;
    std::vector<long> p;
    std::vector<long> v;
    std::vector<long> a;
    std::vector<long> position(long tick = 0) const;
};

std::tuple<long, long> solve(std::istream &is);
std::vector<Particle> parse(std::istream &is);
long part1(std::vector<Particle> const &particles);
long part2(std::vector<Particle> const &particles);

} // namespace day20

// --------
module :private;

namespace day20 {

std::vector<long>
Particle::position(long tick) const {
    auto new_p = [](long pos, long vel, long acc, long t) {
        return pos + t * vel + (t * (t + 1)) * acc / 2;
    };

    return {
        new_p(p[0], v[0], a[0], tick),
        new_p(p[1], v[1], a[1], tick),
        new_p(p[2], v[2], a[2], tick)
    };
}

std::tuple<long, long>
solve(std::istream &is) {
    auto input_data = parse(is);

    return {part1(input_data), part2(input_data)};
}

std::vector<Particle>
parse(std::istream &is) {
    std::vector<Particle> result;

    long id {0};
    for (std::string line; std::getline(is, line); ++id) {
        auto nums = util::getNumbers(line);
        result.push_back(Particle(
            id,
            {nums[0], nums[1], nums[2]},
            {nums[3], nums[4], nums[5]},
            {nums[6], nums[7], nums[8]}
        ));
    }

    return result;
}

// manhattan norm (1-norm)
long
norm(std::vector<long> const &vs) {
    long result {0};
    for (auto const v : vs) {
        result += std::abs(v);
    }

    return result;
}

bool
isFindClosest(std::deque<std::vector<std::pair<long, long>>> const &dq) {
    // ensure that the order of IDs is the same at each timing
    std::vector<std::vector<long>> ids;
    for (auto const &pairs : dq) {
        std::vector<long> tmp;
        for (auto const &p : pairs) {
            tmp.push_back(p.second);
        }
        ids.push_back(tmp);
    }

    auto first_elmnt = ids[0];
    auto order_check = std::ranges::all_of(ids, [&](auto const &vs) {
        return first_elmnt == vs;
    });
    if (!order_check) {
        return false;
    }

    auto n_particles = dq[0].size();

    // Check that moving distances are monotonically increasing on each period
    // work =
    //   [[diff_p0(t2,t1), diff_p0(t3,t2), ...], [diff_p1(t2,t1), diff_p0(t3,t2), ...], ...]
    std::vector<std::vector<long>> work;
    for (auto i = 0uz; i < n_particles; ++i) {
        std::vector<long> vs;
        long prev {0};
        for (auto const &dist_info : dq) {
            vs.push_back(dist_info[i].first - prev);
            prev = dist_info[i].first;
        }
        vs.erase(vs.begin());
        work.push_back(vs);
    }

    auto mono_inc1_check = std::ranges::all_of(work, [](auto const &vs) {
        return std::ranges::is_sorted(vs);
    });
    if (!mono_inc1_check) {
        return false;
    }

    // On each period, check that moving distance by a particle closer to the origin
    // is smaller than that farther away.
    // trans_work =
    //   [[diff_p0(t2,t1), diff_p1(t2,t1), ...], [diff_p0(t3,t2), diff_p1(t3,t2), ...], ...]
    std::vector<std::vector<long>> trans_work;
    for (auto i = 0uz; i < work[0].size(); ++i) {
        std::vector<long> vs;
        for (auto j = 0uz; j < n_particles; ++j) {
            vs.push_back(work[j][i]);
        }
        trans_work.push_back(vs);
    }

    auto mono_inc2_check = std::ranges::all_of(trans_work, [](auto const &vs) {
        return std::ranges::is_sorted(vs);
    });
    if (!mono_inc2_check) {
        return false;
    }

    return true;
}

// Measure manhattan distance from the origin of each particle at the time of `t`,
// and sort them in ascending order
std::vector<std::pair<long, long>>
measureDistances(std::vector<Particle> const &ps, long t) {
    // compare on the first element of a pair, manhattan distance
    auto pair_cmp = [](std::pair<long, long> const &x,
                       std::pair<long, long> const &y) {
        return x.first < y.first;
    };

    std::vector<std::pair<long, long>> distances;
    for (auto const &p : ps) {
        distances.emplace_back(norm(p.position(t)), p.id);
    }
    std::ranges::sort(distances, pair_cmp);

    return distances;
}

long
findClosestParticleId(std::vector<Particle> const &ps) {
    std::deque<std::vector<std::pair<long, long>>> dq;

    // the first N_COUNT times are at 1000, 2000, ... ticks elapsed
    constexpr long N_COUNT {5}; // it must be equal or larger than 3
    constexpr long TICK {1'000};
    long t = TICK;
    while (t <= TICK * N_COUNT) {
        dq.push_back(measureDistances(ps, t));
        t += TICK;
    }

    // repeat with time advancing until the change of distances is stable
    while (!isFindClosest(dq)) {
        dq.pop_front();
        dq.push_back(measureDistances(ps, t));
        t += TICK;
    }

    return dq[0][0].second;
}

long
part1(std::vector<Particle> const &particles) {
    // find candidate particles closest to the origin
    // candidate is the particle with the smallest magnitude of acceleration
    auto min_acc = norm((*std::ranges::min_element(
                             particles,
                             [](Particle const &x, Particle const &y) {
                                 return norm(x.a) < norm(y.a);
                             }
                         )).a);

    std::vector<Particle> cands;
    for (auto const &p : particles) {
        if (norm(p.a) == min_acc) {
            cands.push_back(p);
        }
    }

    // If there are multiple particles with the smallest magnitude of acceleration,
    // determine the closest particle from changes in distances.
    if (cands.size() == 1) {
        return cands[0].id;
    } else {
        return findClosestParticleId(cands);
    }
}

bool
isSquare(long n) {
    if (n < 0) {
        return false;
    } else {
        long const sq_n = static_cast<long>(std::sqrt(n));
        return sq_n * sq_n == n;
    }
}

std::optional<std::vector<long>>
solveQuadratic(long a, long b, long c) {
    std::vector<long> result;

    if (a == 0) {
        if (b == 0) {
            return std::nullopt;
        }
        if (c % b == 0 && -c / b >= 0) {
            result.push_back(-c / b);
        }
    } else {
        auto const D = b * b - 4 * a * c;
        if (!isSquare(D)) {
            return std::nullopt;
        }
        long const sq_D = static_cast<long>(std::sqrt(D));

        if ((-b + sq_D) % (2 * a) == 0) {
            result.push_back((-b + sq_D) / (2 * a));
        }
        if ((-b - sq_D) % (2 * a) == 0) {
            result.push_back((-b - sq_D) / (2 * a));
        }
    }

    // remove past collision times
    std::erase_if(result, [](auto const n) { return n < 0; });

    if (result.size() > 0) {
        return result;
    } else {
        return std::nullopt;
    }
}

std::optional<long>
getCollisionTime(Particle const &p1, Particle const &p2) {
    std::set<long> tmp;

    // i: x, y, z axis
    for (auto i = 0uz; i < 3; ++i) {
        auto dp = p1.p[i] - p2.p[i];
        auto dv = p1.v[i] - p2.v[i];
        auto da = p1.a[i] - p2.a[i];

        // if it can't be determined, move on to the next axis
        if (da == 0 && dv == 0) {
            continue;
        }

        auto collisions = solveQuadratic(da, 2 * dv + da, 2 * dp);
        if (collisions) {
            for (auto const t : collisions.value()) {
                if (p1.position(t) == p2.position(t)) {
                    tmp.insert(t);
                }
            }
        }

        break;
    }

    if (tmp.size() == 0) {
        return std::nullopt;
    } else {
        // choose the earliest timing
        return *std::ranges::min_element(tmp);
    }
}

// key: estimated time of collision
// value: vector of colliding particle pairs
//
// <example>
// key = 7
// value = (26, 27) (26, 28) (27, 28)
std::map<long, std::vector<std::pair<size_t, size_t>>>
makeCollisionMap(std::vector<Particle> const &particles) {
    std::map<long, std::vector<std::pair<size_t, size_t>>> collisions;

    for (auto i = 0uz; i < particles.size() - 1; ++i) {
        for (auto j = i + 1; j < particles.size(); ++j) {
            auto t = getCollisionTime(particles[i], particles[j]);
            if (t) {
                collisions[t.value()].emplace_back(i, j);
            }
        }
    }

    return collisions;
}

long
part2(std::vector<Particle> const &particles) {
    auto collision_map = makeCollisionMap(particles);
    std::vector<bool> survival_tbl(particles.size(), true);
    std::set<size_t> del_particles;

    // since collision_map is a std::map, it can be checked in order of time elapsed
    for (auto const &pairs : std::views::values(collision_map)) {
        del_particles.clear();
        for (auto const &p : pairs) {
            // collision possible if both particles exist in the survival table
            if (survival_tbl[p.first] && survival_tbl[p.second]) {
                del_particles.insert(p.first);
                del_particles.insert(p.second);
            }
        }

        // clear suvival flags of collided particles
        for (auto const idx : del_particles) {
            survival_tbl[idx] = false;
        }
    }

    return std::ranges::count(survival_tbl, true);
}

} // namespace day20
