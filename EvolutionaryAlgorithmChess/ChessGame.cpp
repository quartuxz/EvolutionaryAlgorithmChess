#include "ChessGame.h"

ChessGame::ChessGame()
{
    //make starting board
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t o = 0; o < 8; o++)
        {
            m_current[i][o] = chessPiece::empty;
        }
    }

    m_current[0][0] = chessPiece::blackRook;
    m_current[0][1] = chessPiece::blackKnight;
    m_current[0][2] = chessPiece::blackBishop;
    m_current[0][3] = chessPiece::blackQueen;
    m_current[0][4] = chessPiece::blackKing;
    m_current[0][5] = chessPiece::blackBishop;
    m_current[0][6] = chessPiece::blackKnight;
    m_current[0][7] = chessPiece::blackRook;
    for (size_t i = 0; i < 8; i++)
    {
        m_current[1][i] = chessPiece::blackPawn;
    }


    m_current[7][0] = chessPiece::whiteRook;
    m_current[7][1] = chessPiece::whiteKnight;
    m_current[7][2] = chessPiece::whiteBishop;
    m_current[7][3] = chessPiece::whiteQueen;
    m_current[7][4] = chessPiece::whiteKing;
    m_current[7][5] = chessPiece::whiteBishop;
    m_current[7][6] = chessPiece::whiteKnight;
    m_current[7][7] = chessPiece::whiteRook;

    for (size_t i = 0; i < 8; i++)
    {
        m_current[6][i] = chessPiece::whitePawn;
    }

}

player getPieceColor(chessPiece piece) {
    if ((size_t)piece == 0) {
        return player::neither;
    }
    if ((size_t)piece > 0 && (size_t)piece <= 6) {
        return player::black;
    }
    if ((size_t)piece > 6 && (size_t)piece <= 12) {
        return player::white;
    }
}


board makeMove(board brd, boardCoords coords, chessPiece piece) {
    brd[coords.first][coords.second] = piece;
    return brd;
}

chessPiece makePiece(colorlessChessPiece piece, player whoToPlay) {
    size_t start = 1;
    if (whoToPlay == player::white) {
        start = 7;
    }
    return (chessPiece)((size_t)piece+start);
}   

#define SIMPLE_MOVE(moves, brd, coordsTo, coordsFrom, piece, whoToPlay, moveType) moves.push_back( boardAndPreviousMove{makeMove(brd, coordsTo, makePiece(piece, whoToPlay)), chessMove(moveType,coordsFrom,coordsTo,whoToPlay)})

std::vector<boardAndPreviousMove> getPawnMoves(board brd , boardCoords coords, player whoToPlay) {
    std::vector<boardAndPreviousMove> moves;
    brd[coords.first][coords.second] = chessPiece::empty;
    board tempBoard = brd;

    
    int up = 1;
    chessPiece pawn = chessPiece::whitePawn;
    if (whoToPlay == player::black) {
        chessPiece pawn = chessPiece::blackPawn;
        up = -1;
    }

    if ((whoToPlay == player::white && coords.first == 1) || (whoToPlay == player::black && coords.first == 6)) {
         
        auto tempCoords = boardCoords(coords.first+up, coords.second);
        if (brd[tempCoords.first][tempCoords.second] == chessPiece::empty) {
            SIMPLE_MOVE(moves, brd, tempCoords, coords, colorlessChessPiece::rook, whoToPlay, chessMove::promotion);
            SIMPLE_MOVE(moves, brd, tempCoords, coords, colorlessChessPiece::queen, whoToPlay, chessMove::promotion);
            SIMPLE_MOVE(moves, brd, tempCoords, coords, colorlessChessPiece::bishop, whoToPlay, chessMove::promotion);
            SIMPLE_MOVE(moves, brd, tempCoords, coords, colorlessChessPiece::knight, whoToPlay, chessMove::promotion);
        }
    } 

    
}

std::vector<board> ChessGame::getPossibleBoards()
{
    //calculate all posible boards


    for (size_t i = 0; i < 8; i++)
    {
        for (size_t o = 0; o < 8; o++)
        {
            auto piece = m_current[i][o];

            switch (piece)
            {
            case chessPiece::empty:
                continue;
            case chessPiece::blackPawn:

                break;
            case chessPiece::blackRook:
                break;
            case chessPiece::blackKnight:
                break;
            case chessPiece::blackBishop:
                break;
            case chessPiece::blackKing:
                break;
            case chessPiece::blackQueen:
                break;
            case chessPiece::whitePawn:
                break;
            case chessPiece::whiteRook:
                break;
            case chessPiece::whiteKnight:
                break;
            case chessPiece::whiteBishop:
                break;
            case chessPiece::whiteKing:
                break;
            case chessPiece::whiteQueen:
                break;
            default:
                break;
            }


        }
    }

    return ;
}

void ChessGame::setCurrentBoard(board brd)
{
}

chessMove::chessMove(moveTypes p_moveType, boardCoords p_whereFrom, boardCoords p_whereTo, player p_who):
    moveType(p_moveType),
    whereFrom(p_whereFrom),
    whereTo(p_whereTo),
    who(p_who)
{
}
