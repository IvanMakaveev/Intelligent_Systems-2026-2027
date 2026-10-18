from collections import deque
from heapq import heappop, heappush


def _ancestors(link):
    while link is not None:
        state, link = link
        yield state


def _path(link):
    return list(reversed(list(_ancestors(link))))


def dfs(s, successor, g):
    frontier = [s]
    parents = [None]  # Parent links parallel to the frontier.

    while frontier:
        state = frontier.pop()
        link = (state, parents.pop())

        if state == g:
            return _path(link)

        for next_state, _ in reversed(successor(state)):
            frontier.append(next_state)
            parents.append(link)

    return None


def bfs(s, successor, g):
    frontier = deque([s])
    parents = {s: (s, None)}

    while frontier:
        state = frontier.popleft()

        if state == g:
            return _path(parents[state])

        for next_state, _ in successor(state):
            if next_state not in parents:
                parents[next_state] = (next_state, parents[state])
                frontier.append(next_state)

    return None


def ucs(s, successor, g):
    frontier = [(0, s)]
    costs = {s: 0}
    parents = {s: (s, None)}

    while frontier:
        cost, state = heappop(frontier)

        if cost != costs[state]:
            continue  # Skip an entry superseded by a cheaper one.

        if state == g:
            return _path(parents[state])

        for next_state, step_cost in successor(state):
            new_cost = cost + step_cost

            if new_cost < costs.get(next_state, float("inf")):
                costs[next_state] = new_cost
                parents[next_state] = (next_state, parents[state])
                heappush(frontier, (new_cost, next_state))

    return None


def _dls(s, successor, g, limit):
    frontier = [s]
    metadata = [(None, limit)]  # Parent link and remaining depth.
    cutoff = False

    while frontier:
        state = frontier.pop()
        parent, remaining = metadata.pop()
        link = (state, parent)

        if state == g:
            return _path(link), False

        if remaining == 0:
            cutoff = True
            continue

        for next_state, _ in reversed(successor(state)):
            if next_state not in _ancestors(link):
                frontier.append(next_state)
                metadata.append((link, remaining - 1))

    return None, cutoff


def dls(s, successor, g, limit=10):
    if limit < 0:
        raise ValueError("limit must be nonnegative")

    path, _ = _dls(s, successor, g, limit)
    return path


def ids(s, successor, g):
    limit = 0

    while True:
        path, cutoff = _dls(s, successor, g, limit)

        if path is not None:
            return path
        if not cutoff:
            return None

        limit += 1