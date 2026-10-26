#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

enum class Color { red, green, blue, none };

using Graph = std::unordered_map<std::string, std::vector<std::string>>;
using Assignment = std::unordered_map<std::string, Color>;

const std::vector<Color> colors = {Color::red, Color::green, Color::blue};
const std::vector<std::string> regions = {"WA", "NT", "SA", "Q", "NSW", "V", "T"};

// Select element uniformly at random from a nonempty vector.
template <typename T>
T randomChoice(const std::vector<T>& values, std::mt19937& rng)
{
    std::uniform_int_distribution<std::size_t> pick(0, values.size() - 1);
    return values[pick(rng)];
}

// Count violated constraints if this region were assigned candidateColor given the already made assignments.
int conflicts(const std::string& region, Color candidateColor, const Graph& graph, const Assignment& assignment)
{
    int count = 0;
    for (const std::string& neighbor : graph.at(region))
    {
        if (assignment.at(neighbor) == candidateColor)
        {
            ++count;
        }
    }
    return count;
}

// Start with a complete assignment, then repeatedly repair a randomly chosen 
// conflicted variable using a minimum-conflict value. Random tie-breaking permits 
// sideways moves. The current color is eligible. This bounded local search can 
// fail even when a solution exists.
// Note: this is the textbook version. Different optimizations can be made like:
// 1) Instead of random initialization, use greedy initialization by picking colors with min conflicts for each region.
// 2) Instead of picking a random conflicted region, pick the one with max current conflicts (max-min-conflicts)
// 3) Add randomization for tie breaking the selection of region and/or color.
// 4) Take advantage of the structure of the graph (for example - process separately the connected components).
// 5) Any other optimization we have seen in class.
bool minConflicts(const Graph& graph, Assignment& assignment, int maxSteps, std::mt19937& rng)
{
    for (const std::string& region : regions)
    {
        assignment[region] = randomChoice(colors, rng);
    }

    for (int step = 0; step <= maxSteps; ++step)
    {
        std::vector<std::string> conflicted;
        for (const std::string& region : regions)
        {
            if (conflicts(region, assignment.at(region), graph, assignment) > 0)
            {
                conflicted.push_back(region);
            }
        }

        if (conflicted.empty())
        {
            return true;
        }

        if (step == maxSteps)
        {
            return false;
        }

        const std::string region = randomChoice(conflicted, rng);
        int minimum = std::numeric_limits<int>::max();
        std::vector<Color> bestColors;

        for (Color color : colors)
        {
            const int count = conflicts(region, color, graph, assignment);
            if (count < minimum)
            {
                minimum = count;
                bestColors.clear();
                bestColors.push_back(color);
            } 
            else if (count == minimum)
            {
                bestColors.push_back(color);
            }
        }

        assignment[region] = randomChoice(bestColors, rng);
    }
    return false;
}

const char* colorName(Color color)
{
    switch (color)
    {
        case Color::red:   return "red";
        case Color::green: return "green";
        case Color::blue:  return "blue";
        case Color::none:  return "none";
    }
    return "unknown";
}

int main()
{
    // WA: Western Australia; NT: Northern Territory; SA: South Australia;
    // Q: Queensland; NSW: New South Wales; V: Victoria; T: Tasmania.
    const Graph graph =
    {
        {"WA",  {"NT", "SA"}},
        {"NT",  {"WA", "SA", "Q"}},
        {"SA",  {"WA", "NT", "Q", "NSW", "V"}},
        {"Q",   {"NT", "SA", "NSW"}},
        {"NSW", {"SA", "Q", "V"}},
        {"V",   {"SA", "NSW"}},
        {"T",   {}}
    };

    Assignment assignment;
    for (const std::string& region : regions)
    {
        assignment[region] = Color::none;
    }
    
    std::mt19937 rng(std::random_device{}());
    const int maxSteps = 10000;
    if (!minConflicts(graph, assignment, maxSteps, rng))
    {
        std::cout << "No solution found within " << maxSteps << " repair steps.";
         // This does not prove that the CSP has no solution!!!!!!!
        return 1;
    }

    std::cout << "Valid coloring:" << std::endl;
    for (const std::string& region : regions)
    {
        std::cout << region << ": " << colorName(assignment.at(region)) << std::endl;
    }
    return 0;
}
