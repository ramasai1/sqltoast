/*
 * Use and distribution licensed under the Apache license version 2.
 *
 * See the COPYING file in the root project directory for full text.
 */

#include <vector>

#include "context.h"
#include "error.h"
#include "parser/comment.h"
#include "parser/identifier.h"
#include "parser/lexer.h"
#include "parser/literal.h"
#include "parser/keyword.h"
#include "parser/punctuator.h"
#include "parser/token.h"

namespace sqltoast {

void fill_lexeme(token_t* tok, lexeme_t& lexeme) {
    lexeme.start = tok->lexeme.start;
    lexeme.end = tok->lexeme.end;
}

parse_position_t skip_simple_comments(parse_position_t cursor) {
    parse_position_t start = cursor;
    if (*cursor != '-')
        return start;

    cursor++;
    if (*cursor != '-') {
        return start;
    }

    // The comment content is from the cursor until we find a newline or EOS
    do {
        cursor++;
    } while (*cursor != '\0' && *cursor != '\n');
    return cursor;
}

static size_t NUM_TOKENIZERS = 5;
static tokenize_func_t tokenizers[5] = {
    &token_comment,
    &token_punctuator,
    &token_keyword,
    &token_literal,
    &token_identifier
};

symbol_t lexer_t::peek() const {
    parse_position_t cur = cursor;
    // Advance the lexer's cursor over any whitespace or simple comments
    while (std::isspace(*cur))
        cur++;
    cur = skip_simple_comments(cur);

    for (size_t x = 0; x < NUM_TOKENIZERS; x++) {
        auto tok_res = tokenizers[x](cur);
        if (tok_res.code == TOKEN_NOT_FOUND)
            continue;
        if (tok_res.code == TOKEN_FOUND)
            return tok_res.token.symbol;
        // There was an error in tokenizing... return some error marker?
        return SYMBOL_EOS;
    }
    // No more tokens
    return SYMBOL_EOS;
}

token_t* lexer_t::next() {
    parse_position_t cur = cursor;
    // Advance the lexer's cursor over any whitespace or simple comments
    while (std::isspace(*cur))
        cur++;
    cur = skip_simple_comments(cur);

    for (size_t x = 0; x < NUM_TOKENIZERS; x++) {
        auto tok_res = tokenizers[x](cur);
        if (tok_res.code == TOKEN_NOT_FOUND)
            continue;
        if (tok_res.code == TOKEN_FOUND) {
            current_token = tok_res.token;
            cursor = tok_res.token.lexeme.end;
            cursor++;
            return &current_token;
        }
        // There was an error in tokenizing
        return NULL;
    }
    // No more tokens
    return NULL;
}

} // namespace sqltoast
