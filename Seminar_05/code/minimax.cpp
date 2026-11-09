#include <vector>

struct GameState
{
    // Some metadata...
    char moveLabel = ' ';
    
    bool isTerminal = false;
    int utilityScore = 0;
    vector<GameState*> nextMoves;
}

pair<int, char> minimize(GameState* state, int alpha, int beta)
{
    if (state->isTerminal)
    {
        return {state->utilityScore, state->moveLabel};
    }

    int minScore = INT_MAX;
    char bestMove = ' ';
    for (GameState* nextState : state->nextMoves)
    {
        [score, move] = maximize(nextState, alpha, beta);
        if (score < minScore)
        {
            minScore = score;
            bestMove = nextState->moveLabel;
        }

        beta = min(beta, score);
        if (beta <= alpha) // or if (score <= alpha)
        {
            break; 
        }
    }

    return {minScore, bestMove};
}

pair<int, char> maximize(GameState* state, int alpha, int beta)
{
    if (state->isTerminal)
    {
        return {state->utilityScore, state->moveLabel};
    }

    int maxScore = INT_MIN;
    char bestMove = ' ';
    for (GameState* nextState : state->nextMoves)
    {
        [score, move] = minimize(nextState, alpha, beta);
        if (score > maxScore)
        {
            maxScore = score;
            bestMove = nextState->moveLabel;
        }

        alpha = max(alpha, score);
        if (beta <= alpha) // or if (beta <= score)
        {
            break; 
        }
    }

    return {maxScore, bestMove};
}

int minimax(GameState* root)
{
    int alpha = INT_MIN;
    int beta = INT_MAX;

    [maxScore, bestMove] = maximize(root, alpha, beta);
    return bestMove;
}