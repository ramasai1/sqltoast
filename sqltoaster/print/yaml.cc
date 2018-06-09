/*
 * Use and distribution licensed under the Apache license version 2.
 *
 * See the COPYING file in the root project directory for full text.
 */

#include <sqltoast/print.h>

#include "yaml.h"

namespace sqltoaster {
namespace print {

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::statement_t& stmt) {
    if (ptr.in_list(out)) {
        ptr.indent(out) << "- type: ";
        ptr.end_list(out);
    }
    else
        ptr.indent(out) << "type: ";
    ptr.indent_push(out);
    switch (stmt.type) {
        case sqltoast::STATEMENT_TYPE_CREATE_SCHEMA:
            out << "CREATE_SCHEMA";
            {
                const sqltoast::create_schema_statement_t& sub =
                    static_cast<const sqltoast::create_schema_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_DROP_SCHEMA:
            out << "DROP_SCHEMA";
            {
                const sqltoast::drop_schema_statement_t& sub =
                    static_cast<const sqltoast::drop_schema_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_CREATE_TABLE:
            out << "CREATE_TABLE";
            {
                const sqltoast::create_table_statement_t& sub =
                    static_cast<const sqltoast::create_table_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_DROP_TABLE:
            out << "DROP_TABLE";
            {
                const sqltoast::drop_table_statement_t& sub =
                    static_cast<const sqltoast::drop_table_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_ALTER_TABLE:
            out << "ALTER_TABLE";
            {
                const sqltoast::alter_table_statement_t& sub =
                    static_cast<const sqltoast::alter_table_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_CREATE_VIEW:
            out << "CREATE_VIEW";
            {
                const sqltoast::create_view_statement_t& sub =
                    static_cast<const sqltoast::create_view_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_DROP_VIEW:
            out << "DROP_VIEW";
            {
                const sqltoast::drop_view_statement_t& sub =
                    static_cast<const sqltoast::drop_view_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_SELECT:
            out << "SELECT";
            {
                const sqltoast::select_statement_t& sub =
                    static_cast<const sqltoast::select_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_INSERT:
            out << "INSERT";
            {
                const sqltoast::insert_statement_t& sub =
                    static_cast<const sqltoast::insert_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_DELETE:
            out << "DELETE";
            {
                const sqltoast::delete_statement_t& sub =
                    static_cast<const sqltoast::delete_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_UPDATE:
            out << "UPDATE";
            {
                const sqltoast::update_statement_t& sub =
                    static_cast<const sqltoast::update_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STATEMENT_TYPE_COMMIT:
            out << "COMMIT";
            break;
        case sqltoast::STATEMENT_TYPE_ROLLBACK:
            out << "ROLLBACK";
            break;
        case sqltoast::STATEMENT_TYPE_GRANT:
            out << "GRANT";
            {
                const sqltoast::grant_statement_t& sub =
                    static_cast<const sqltoast::grant_statement_t&>(stmt);
                to_yaml(ptr, out, sub);
            }
            break;
        default:
            break;
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::create_schema_statement_t& stmt) {
    ptr.indent(out) << "create_schema_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "schema_name: " << stmt.schema_name;
    if (stmt.authorization_identifier) {
       ptr.indent(out) << "authorization_identifier: " << stmt.authorization_identifier;
    }
    if (stmt.default_charset) {
       ptr.indent(out) << "default_charset: " << stmt.default_charset;
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::drop_schema_statement_t& stmt) {
    ptr.indent(out) << "drop_schema_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "schema_name: " << stmt.schema_name;
    if (stmt.drop_behaviour == sqltoast::DROP_BEHAVIOUR_CASCADE)
       ptr.indent(out) << "drop_behaviour: CASCADE";
    else
       ptr.indent(out) << "drop_behaviour: RESTRICT";
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::create_table_statement_t& stmt) {
    ptr.indent(out) << "create_table_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "table_name: " << stmt.table_name;
    if (stmt.table_type != sqltoast::TABLE_TYPE_NORMAL) {
        ptr.indent(out) << "temporary: true (";
        if (stmt.table_type == sqltoast::TABLE_TYPE_TEMPORARY_GLOBAL)
            out << "global)";
        else
            out << "local)";
    }
    ptr.indent(out) << "column_definitions:";
    ptr.indent_push(out);
    for (auto cdef_it = stmt.column_definitions.cbegin();
            cdef_it != stmt.column_definitions.cend();
            cdef_it++) {
        const sqltoast::column_definition_t& cdef = *(*cdef_it);
        ptr.indent(out) << cdef.name << ": ";
        if (cdef.data_type.get()) {
            out << *cdef.data_type;
        } else {
            out << " UNKNOWN";
        }
        if (cdef.default_descriptor.get()) {
            out << " " << *cdef.default_descriptor;
        }
        for (const std::unique_ptr<sqltoast::constraint_t>& c: cdef.constraints)
            out << *c;
        if (cdef.collate) {
            out << " COLLATE " << cdef.collate;
        }
    }
    ptr.indent_pop(out);
    if (stmt.constraints.size() > 0) {
        ptr.indent(out) << "constraints:";
        ptr.indent_push(out);
        for (auto constraint_it = stmt.constraints.begin();
             constraint_it != stmt.constraints.end();
             constraint_it++) {
            ptr.indent(out) << "- " << *(*constraint_it);
        }
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::drop_table_statement_t& stmt) {
    ptr.indent(out) << "drop_table_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "table_name: " << stmt.table_name;
    if (stmt.drop_behaviour == sqltoast::DROP_BEHAVIOUR_CASCADE)
       ptr.indent(out) << "drop_behaviour: CASCADE";
    else
       ptr.indent(out) << "drop_behaviour: RESTRICT";
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::add_column_action_t& action) {
    out << "ADD COLUMN " << *action.column_definition;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::alter_column_action_t& action) {
    if (action.alter_column_action_type == sqltoast::ALTER_COLUMN_ACTION_TYPE_SET_DEFAULT)
        out << "ALTER COLUMN " << action.column_name
            << " SET " << *action.default_descriptor;
    else
        out << "ALTER COLUMN " << action.column_name
            << " DROP DEFAULT";
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::drop_column_action_t& action) {
    out << "DROP COLUMN " << action.column_name;
    if (action.drop_behaviour == sqltoast::DROP_BEHAVIOUR_CASCADE)
        out << " CASCADE";
    else
        out << " RESTRICT";
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::add_constraint_action_t& action) {
    out << "ADD" << *action.constraint;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::drop_constraint_action_t& action) {
    out << "DROP CONSTRAINT " << action.constraint_name;
    if (action.drop_behaviour == sqltoast::DROP_BEHAVIOUR_CASCADE)
        out << " CASCADE";
    else
        out << " RESTRICT";
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::alter_table_action_t& action) {
    switch (action.type) {
        case sqltoast::ALTER_TABLE_ACTION_TYPE_ADD_COLUMN:
            {
                const sqltoast::add_column_action_t& sub =
                    static_cast<const sqltoast::add_column_action_t&>(action);
                out << sub;
            }
            break;
        case sqltoast::ALTER_TABLE_ACTION_TYPE_ALTER_COLUMN:
            {
                const sqltoast::alter_column_action_t& sub =
                    static_cast<const sqltoast::alter_column_action_t&>(action);
                out << sub;
            }
            break;
        case sqltoast::ALTER_TABLE_ACTION_TYPE_DROP_COLUMN:
            {
                const sqltoast::drop_column_action_t& sub =
                    static_cast<const sqltoast::drop_column_action_t&>(action);
                out << sub;
            }
            break;
        case sqltoast::ALTER_TABLE_ACTION_TYPE_ADD_CONSTRAINT:
            {
                const sqltoast::add_constraint_action_t& sub =
                    static_cast<const sqltoast::add_constraint_action_t&>(action);
                out << sub;
            }
            break;
        case sqltoast::ALTER_TABLE_ACTION_TYPE_DROP_CONSTRAINT:
            {
                const sqltoast::drop_constraint_action_t& sub =
                    static_cast<const sqltoast::drop_constraint_action_t&>(action);
                out << sub;
            }
            break;
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::alter_table_statement_t& stmt) {
    ptr.indent(out) << "alter_table_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "table_name: " << stmt.table_name;
    ptr.indent(out) << "action: " << *stmt.action;
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::create_view_statement_t& stmt) {
    ptr.indent(out) << "create_view_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "view_name: " << stmt.table_name;
    if (! stmt.columns.empty()) {
        ptr.indent(out) << "columns:";
        ptr.indent_push(out);
        for (const auto& column : stmt.columns)
            ptr.indent(out) << "- " << column;
        ptr.indent_pop(out);
    }
    if (stmt.check_option != sqltoast::CHECK_OPTION_NONE) {
        if (stmt.check_option == sqltoast::CHECK_OPTION_LOCAL)
            ptr.indent(out) << "check_option: LOCAL";
        else
            ptr.indent(out) << "check_option: CASCADED";
    }
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *stmt.query);
    ptr.indent_pop(out);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::query_specification_t& query) {
    if (query.distinct)
       ptr.indent(out) << "distinct: true";
    ptr.indent(out) << "selected_columns:";
    ptr.indent_push(out);
    for (const sqltoast::derived_column_t& dc : query.selected_columns) {
        ptr.start_list(out);
        to_yaml(ptr, out, dc);
    }
    ptr.indent_pop(out);
    to_yaml(ptr, out, *query.table_expression);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::derived_column_t& dc) {
    // Need to save whether this was a list item so we can properly de-indent
    // at the end
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- derived_column:";
        ptr.end_list(out);
        ptr.indent_push(out);
    } else {
        ptr.indent(out) << "derived_column:";
    }
    ptr.indent_push(out);
    if (dc.value) {
        to_yaml(ptr, out, *dc.value);
        if (dc.alias)
            ptr.indent(out) << "alias: " << dc.alias;
    }
    else
        // NOTE(jaypipes): the * cannot be aliased in a production
        ptr.indent(out) << "asterisk: true";
    ptr.indent_pop(out);
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::table_expression_t& table_exp) {
    ptr.indent(out) << "referenced_tables:";
    ptr.indent_push(out);
    for (const std::unique_ptr<sqltoast::table_reference_t>& tr : table_exp.referenced_tables) {
        ptr.start_list(out);
        to_yaml(ptr, out, *tr);
    }
    ptr.indent_pop(out);
    if (table_exp.where_condition) {
        ptr.indent(out) << "where:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *table_exp.where_condition);
        ptr.indent_pop(out);
    }
    if (! table_exp.group_by_columns.empty()) {
        ptr.indent(out) << " group_by:";
        ptr.indent_push(out);
        for (const sqltoast::grouping_column_reference_t& gcr : table_exp.group_by_columns)
            ptr.indent(out) << "- " << gcr;
        ptr.indent_pop(out);
    }
    if (table_exp.having_condition)
        ptr.indent(out) << "having: " << *table_exp.having_condition;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::query_expression_t& qe) {
    switch (qe.query_expression_type) {
        case sqltoast::QUERY_EXPRESSION_TYPE_NON_JOIN_QUERY_EXPRESSION:
            {
                const sqltoast::non_join_query_expression_t& sub =
                    static_cast<const sqltoast::non_join_query_expression_t&>(qe);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::QUERY_EXPRESSION_TYPE_JOINED_TABLE:
            {
                const sqltoast::joined_table_query_expression_t& sub =
                    static_cast<const sqltoast::joined_table_query_expression_t&>(qe);
                to_yaml(ptr, out, sub);
            }
            break;
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::non_join_query_expression_t& qe) {
    to_yaml(ptr, out, *qe.term);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::non_join_query_term_t& term) {
    to_yaml(ptr, out, *term.primary);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::non_join_query_primary_t& primary) {
    switch (primary.primary_type) {
        case sqltoast::NON_JOIN_QUERY_PRIMARY_TYPE_QUERY_SPECIFICATION:
            {
                const sqltoast::query_specification_non_join_query_primary_t& sub =
                    static_cast<const sqltoast::query_specification_non_join_query_primary_t&>(primary);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::NON_JOIN_QUERY_PRIMARY_TYPE_TABLE_VALUE_CONSTRUCTOR:
            {
                const sqltoast::table_value_constructor_non_join_query_primary_t& sub =
                    static_cast<const sqltoast::table_value_constructor_non_join_query_primary_t&>(primary);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::NON_JOIN_QUERY_PRIMARY_TYPE_EXPLICIT_TABLE:
        case sqltoast::NON_JOIN_QUERY_PRIMARY_TYPE_SUBEXPRESSION:
            // TODO
            break;
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::query_specification_non_join_query_primary_t& primary) {
    to_yaml(ptr, out, *primary.query_spec);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::table_value_constructor_non_join_query_primary_t& primary) {
    ptr.indent(out) << "values:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *primary.table_value);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::table_value_constructor_t& table_value) {
    for (const std::unique_ptr<sqltoast::row_value_constructor_t>& value : table_value.values) {
        ptr.start_list(out);
        to_yaml(ptr, out, *value);
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::joined_table_query_expression_t& qe) {
    to_yaml(ptr, out, *qe.joined_table);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::table_reference_t& tr) {
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- type: ";
        ptr.indent_push(out);
        ptr.end_list(out);
    }
    else
        ptr.indent(out) << "type: ";
    switch (tr.type) {
        case sqltoast::TABLE_REFERENCE_TYPE_TABLE:
            out << "TABLE";
            {
                const sqltoast::table_t& sub =
                    static_cast<const sqltoast::table_t&>(tr);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::TABLE_REFERENCE_TYPE_DERIVED_TABLE:
            out << "DERIVED_TABLE";
            {
                const sqltoast::derived_table_t& sub =
                    static_cast<const sqltoast::derived_table_t&>(tr);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::TABLE_REFERENCE_TYPE_JOINED_TABLE:
            out << "JOINED_TABLE";
            {
                const sqltoast::joined_table_t& sub =
                    static_cast<const sqltoast::joined_table_t&>(tr);
                to_yaml(ptr, out, sub);
            }
            break;
    }
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::table_t& t) {
    ptr.indent(out) << "table:";
    ptr.indent_push(out);
    ptr.indent(out) << "name: " << t.table_name;
    if (t.has_alias())
        ptr.indent(out) << "alias: " << t.alias;
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::derived_table_t& t) {
    ptr.indent(out) << "derived_table:";
    ptr.indent_push(out);
    ptr.indent(out) << "name: " << t.table_name;
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *t.query);
    ptr.indent_pop(out);
    ptr.indent_pop(out);
    // NOTE(jaypipes): derived tables don't have aliases because they are
    // required to be named with AS <table name>
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::joined_table_t& jt) {
    ptr.indent(out) << "joined_table:";
    ptr.indent_push(out);
    ptr.indent(out) << "join_type: ";
    switch (jt.join_type) {
        case sqltoast::JOIN_TYPE_INNER:
            out << "INNER";
            break;
        case sqltoast::JOIN_TYPE_LEFT:
            out << "LEFT";
            break;
        case sqltoast::JOIN_TYPE_RIGHT:
            out << "RIGHT";
            break;
        case sqltoast::JOIN_TYPE_FULL:
            out << "FULL";
            break;
        case sqltoast::JOIN_TYPE_CROSS:
            out << "CROSS";
            break;
        case sqltoast::JOIN_TYPE_NATURAL:
            out << "NATURAL";
            break;
        case sqltoast::JOIN_TYPE_UNION:
            out << "UNION";
            break;
        default:
            break;
    }
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *jt.left);
    ptr.indent_pop(out);
    ptr.indent(out) << "right:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *jt.right);
    ptr.indent_pop(out);
    if (jt.spec)
       to_yaml(ptr, out, *jt.spec);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::join_specification_t& spec) {
    ptr.indent(out) << "specification:";
    ptr.indent_push(out);
    if (spec.condition) {
        ptr.indent(out) << "on:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *spec.condition);
        ptr.indent_pop(out);
    } else if (! spec.named_columns.empty()) {
        ptr.indent(out) << "using:";
        ptr.indent_push(out);
        for (const sqltoast::lexeme_t& col : spec.named_columns)
            ptr.indent(out) << "- " << col;
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::drop_view_statement_t& stmt) {
    ptr.indent(out) << "drop_view_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "view_name: " << stmt.table_name;
    if (stmt.drop_behaviour == sqltoast::DROP_BEHAVIOUR_CASCADE)
       ptr.indent(out) << "drop_behaviour: CASCADE";
    else
       ptr.indent(out) << "drop_behaviour: RESTRICT";
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::select_statement_t& stmt) {
    ptr.indent(out) << "select_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *stmt.query);
    ptr.indent_pop(out);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::insert_statement_t& stmt) {
    ptr.indent(out) << "insert_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "table_name: " << stmt.table_name;
    if (! stmt.insert_columns.empty()) {
        ptr.indent(out) << "columns:";
        ptr.indent_push(out);
        for (const sqltoast::lexeme_t& col : stmt.insert_columns)
            ptr.indent(out) << "- " << col;
        ptr.indent_pop(out);
    }
    if (stmt.query) {
        ptr.indent(out) << "query:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *stmt.query);
        ptr.indent_pop(out);
        ptr.indent_pop(out);
    } else {
        ptr.indent(out) << "default_values: true";
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::delete_statement_t& stmt) {
    ptr.indent(out) << "delete_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "table_name: " << stmt.table_name;
    if (stmt.where_condition) {
        ptr.indent(out) << "where:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *stmt.where_condition);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::update_statement_t& stmt) {
    ptr.indent(out) << "update_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "table_name: " << stmt.table_name;
    ptr.indent(out) << "set_columns:";
    ptr.indent_push(out);
    for (const sqltoast::set_column_t& set_col : stmt.set_columns) {
        ptr.indent(out) << set_col.column_name << ": ";
        if (set_col.type == sqltoast::SET_COLUMN_TYPE_NULL)
            out << "NULL";
        else if (set_col.type == sqltoast::SET_COLUMN_TYPE_DEFAULT)
            out << "DEFAULT";
        else
            out << *set_col.value;
    }
    ptr.indent_pop(out);

    if (stmt.where_condition) {
        ptr.indent(out) << "where:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *stmt.where_condition);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::grant_action_t& action) {
    switch (action.type) {
        case sqltoast::GRANT_ACTION_TYPE_SELECT:
            out << "SELECT";
            return;
        case sqltoast::GRANT_ACTION_TYPE_DELETE:
            out << "DELETE";
            return;
        case sqltoast::GRANT_ACTION_TYPE_USAGE:
            out << "USAGE";
            return;
        case sqltoast::GRANT_ACTION_TYPE_INSERT:
            out << "INSERT";
            break;
        case sqltoast::GRANT_ACTION_TYPE_UPDATE:
            out << "UPDATE";
            break;
        case sqltoast::GRANT_ACTION_TYPE_REFERENCES:
            out << "REFERENCES";
            break;
    }
    const sqltoast::column_list_grant_action_t& cla =
        static_cast<const sqltoast::column_list_grant_action_t&>(action);
    if (cla.columns.empty())
        return;
    out << " (";
    size_t x = 0;
    for (const sqltoast::lexeme_t& col : cla.columns) {
        if (x++ > 0)
            out << ',';
        out << col;
    }
    out << ')';
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::grant_statement_t& stmt) {
    ptr.indent(out) << "grant_statement:";
    ptr.indent_push(out);
    ptr.indent(out) << "on: ";
    switch (stmt.object_type) {
        case sqltoast::GRANT_OBJECT_TYPE_TABLE:
            break;
        case sqltoast::GRANT_OBJECT_TYPE_DOMAIN:
            out << "DOMAIN ";
            break;
        case sqltoast::GRANT_OBJECT_TYPE_COLLATION:
            out << "COLLATION ";
            break;
        case sqltoast::GRANT_OBJECT_TYPE_CHARACTER_SET:
            out << "CHARACTER SET ";
            break;
        case sqltoast::GRANT_OBJECT_TYPE_TRANSLATION:
            out << "TRANSLATION ";
            break;
    }
    out << stmt.on;
    if (stmt.to_public())
        ptr.indent(out) << "to: PUBLIC";
    else
        ptr.indent(out) << "to: " << stmt.to;
    if (stmt.with_grant_option)
        ptr.indent(out) << "with_grant_option: YES";
    if (stmt.all_privileges()) {
        ptr.indent(out) << "privileges:";
        ptr.indent_push(out);
        ptr.indent(out) << "- ALL";
        ptr.indent_pop(out);
    } else {
        ptr.indent(out) << "privileges:";
        ptr.indent_push(out);
        for (const std::unique_ptr<sqltoast::grant_action_t>& action : stmt.privileges)
            ptr.indent(out) << "- " << *action;
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::search_condition_t& sc) {
    // OR'd operands are on the same "level" as each other for evaluation
    // purposes, which is why we don't attempt to indent here and just
    // output the terms in a list
    ptr.indent(out) << "search_condition:";
    ptr.indent_push(out);
    ptr.indent(out) << "terms:";
    ptr.indent_push(out);
    for (const std::unique_ptr<sqltoast::boolean_term_t>& or_term_p : sc.terms) {
        ptr.start_list(out);
        to_yaml(ptr, out, *or_term_p);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::predicate_t& pred) {
    ptr.indent(out) << "predicate:";
    ptr.indent_push(out);
    ptr.indent(out) << "type: ";
    switch (pred.predicate_type) {
        case sqltoast::PREDICATE_TYPE_COMPARISON:
            out << "COMPARISON";
            {
                const sqltoast::comp_predicate_t& sub =
                    static_cast<const sqltoast::comp_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_BETWEEN:
            out << "BETWEEN";
            {
                const sqltoast::between_predicate_t& sub =
                    static_cast<const sqltoast::between_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_LIKE:
            out << "LIKE";
            {
                const sqltoast::like_predicate_t& sub =
                    static_cast<const sqltoast::like_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_NULL:
            out << "NULL";
            {
                const sqltoast::null_predicate_t& sub =
                    static_cast<const sqltoast::null_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_IN_VALUES:
            out << "IN_VALUES";
            {
                const sqltoast::in_values_predicate_t& sub =
                    static_cast<const sqltoast::in_values_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_IN_SUBQUERY:
            out << "IN_SUBQUERY";
            {
                const sqltoast::in_subquery_predicate_t& sub =
                    static_cast<const sqltoast::in_subquery_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_QUANTIFIED_COMPARISON:
            out << "QUANTIFIED_COMPARISON";
            {
                const sqltoast::quantified_comparison_predicate_t& sub =
                    static_cast<const sqltoast::quantified_comparison_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_EXISTS:
            out << "EXISTS";
            {
                const sqltoast::exists_predicate_t& sub =
                    static_cast<const sqltoast::exists_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_UNIQUE:
            out << "UNIQUE";
            {
                const sqltoast::unique_predicate_t& sub =
                    static_cast<const sqltoast::unique_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_MATCH:
            out << "MATCH";
            {
                const sqltoast::match_predicate_t& sub =
                    static_cast<const sqltoast::match_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::PREDICATE_TYPE_OVERLAPS:
            out << "OVERLAPS";
            {
                const sqltoast::overlaps_predicate_t& sub =
                    static_cast<const sqltoast::overlaps_predicate_t&>(pred);
                to_yaml(ptr, out, sub);
            }
            break;
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::comp_op_t& op) {
    ptr.indent(out) << "op: ";
    switch (op) {
        case sqltoast::COMP_OP_EQUAL:
            out << "EQUAL";
            break;
        case sqltoast::COMP_OP_NOT_EQUAL:
            out << "NOT_EQUAL";
            break;
        case sqltoast::COMP_OP_LESS:
            out << "LESS_THAN";
            break;
        case sqltoast::COMP_OP_GREATER:
            out << "GREATER_THAN";
            break;
        case sqltoast::COMP_OP_LESS_EQUAL:
            out << "LESS_THAN_OR_EQUAL";
            break;
        case sqltoast::COMP_OP_GREATER_EQUAL:
            out << "GREATER_THAN_OR_EQUAL";
            break;
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::comp_predicate_t& pred) {
    to_yaml(ptr, out, pred.op);
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    ptr.indent(out) << "right:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.right);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::between_predicate_t& pred) {
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    ptr.indent(out) << "comp_left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.comp_left);
    ptr.indent_pop(out);
    ptr.indent(out) << "comp_right:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.comp_right);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::like_predicate_t& pred) {
    ptr.indent(out) << "match:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.match);
    ptr.indent_pop(out);
    if (pred.reverse_op)
        ptr.indent(out) << "negate: true";
    ptr.indent(out) << "pattern:";
    ptr.indent_push(out);
    const sqltoast::character_value_expression_t& pattern_val =
        static_cast<const sqltoast::character_value_expression_t&>(*pred.pattern);
    to_yaml(ptr, out, pattern_val);
    ptr.indent_pop(out);
    if (pred.escape_char) {
        const sqltoast::character_value_expression_t& escape_char_val =
            static_cast<const sqltoast::character_value_expression_t&>(*pred.escape_char);
        ptr.indent(out) << "escape:";
        ptr.indent_push(out);
        to_yaml(ptr, out, escape_char_val);
        ptr.indent_pop(out);
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::null_predicate_t& pred) {
    if (pred.reverse_op)
        ptr.indent(out) << "negate: true";
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::in_values_predicate_t& pred) {
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    if (pred.reverse_op)
        ptr.indent(out) << "negate: true";
    ptr.indent(out) << "values:";
    ptr.indent_push(out);
    for (auto& ve : pred.values) {
        ptr.start_list(out);
        to_yaml(ptr, out, *ve);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::in_subquery_predicate_t& pred) {
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.subquery);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::quantified_comparison_predicate_t& pred) {
    to_yaml(ptr, out, pred.op);
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    if (pred.quantifier == sqltoast::QUANTIFIER_ALL)
        ptr.indent(out) << "quantifier: ALL";
    else
        ptr.indent(out) << "quantifier: ANY";
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.subquery);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::exists_predicate_t& pred) {
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.subquery);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::unique_predicate_t& pred) {
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.subquery);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::match_predicate_t& pred) {
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    if (pred.match_unique)
        ptr.indent(out) << "unique: true";
    if (pred.match_partial)
        ptr.indent(out) << "partial: true";
    ptr.indent(out) << "query:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.subquery);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::overlaps_predicate_t& pred) {
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.left);
    ptr.indent_pop(out);
    ptr.indent(out) << "right:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *pred.right);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::boolean_primary_t& bp) {
    if (bp.predicate)
        to_yaml(ptr, out, *bp.predicate);
    else
        to_yaml(ptr, out, *bp.search_condition);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::boolean_factor_t& bf) {
    to_yaml(ptr, out, *bf.primary);
    if (bf.reverse_op)
        ptr.indent(out) << "negate: true";
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::boolean_term_t& bt) {
    // Need to save whether this was a list item so we can properly de-indent
    // at the end
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- factor:";
        ptr.end_list(out);
        ptr.indent_push(out);
    } else {
        ptr.indent(out) << "factor:";
    }
    ptr.indent_push(out);
    to_yaml(ptr, out, *bt.factor);
    ptr.indent_pop(out);
    if (bt.and_operand) {
        ptr.indent(out) << "and:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *bt.and_operand);
        ptr.indent_pop(out);
    }
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::row_value_constructor_t& rvc) {
    // Need to save whether this was a list item so we can properly de-indent
    // at the end
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- row_value_constructor:";
        ptr.end_list(out);
        ptr.indent_push(out);
    } else {
        ptr.indent(out) << "row_value_constructor:";
    }
    ptr.indent_push(out);
    ptr.indent(out) << "type: ";
    switch (rvc.rvc_type) {
        case sqltoast::RVC_TYPE_ELEMENT:
            out << "ELEMENT";
            ptr.indent(out) << "element:";
            ptr.indent_push(out);
            {
                const sqltoast::row_value_constructor_element_t& sub =
                    static_cast<const sqltoast::row_value_constructor_element_t&>(rvc);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            break;
        case sqltoast::RVC_TYPE_LIST:
            out << "LIST";
            ptr.indent(out) << "elements:";
            ptr.indent_push(out);
            {
                const sqltoast::row_value_constructor_list_t& sub =
                    static_cast<const sqltoast::row_value_constructor_list_t&>(rvc);
                for (const auto& rvc_el : sub.elements) {
                    const sqltoast::row_value_constructor_element_t& el =
                        static_cast<const sqltoast::row_value_constructor_element_t&>(*rvc_el);
                    ptr.start_list(out);
                    to_yaml(ptr, out, el);
                }
            }
            ptr.indent_pop(out);
            break;
        case sqltoast::RVC_TYPE_SUBQUERY:
            out << "SUBQUERY";
            // TODO
            break;
    }
    ptr.indent_pop(out);
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::row_value_constructor_element_t& rvce) {
    // Need to save whether this was a list item so we can properly de-indent
    // at the end
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- type: ";
        ptr.end_list(out);
        ptr.indent_push(out);
    } else {
        ptr.indent(out) << "type: ";
    }
    switch (rvce.rvc_element_type) {
        case sqltoast::RVC_ELEMENT_TYPE_DEFAULT:
            out << "DEFAULT";
            break;
        case sqltoast::RVC_ELEMENT_TYPE_NULL:
            out << "NULL";
            break;
        case sqltoast::RVC_ELEMENT_TYPE_VALUE_EXPRESSION:
            out << "VALUE_EXPRESSION";
            {
                const sqltoast::row_value_expression_t& val =
                    static_cast<const sqltoast::row_value_expression_t&>(rvce);
                to_yaml(ptr, out, val);
            }
            break;
    }
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::row_value_expression_t& rve) {
    // row_value_expressions only have a single attribute -- the value, so just
    // pass through to that and condense the YAML output accordingly
    to_yaml(ptr, out, *rve.value);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::value_expression_t& ve) {
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- value_expression:";
        ptr.end_list(out);
        ptr.indent_push(out);
    } else {
        ptr.indent(out) << "value_expression:";
    }
    ptr.indent_push(out);
    ptr.indent(out) << "type: ";
    switch (ve.type) {
        case sqltoast::VALUE_EXPRESSION_TYPE_NUMERIC_EXPRESSION:
            out << "NUMERIC_EXPRESSION";
            {
                const sqltoast::numeric_expression_t& sub =
                    static_cast<const sqltoast::numeric_expression_t&>(ve);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::VALUE_EXPRESSION_TYPE_STRING_EXPRESSION:
            out << "STRING_EXPRESSION";
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(ve);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::VALUE_EXPRESSION_TYPE_DATETIME_EXPRESSION:
            out << "DATETIME_EXPRESSION";
            {
                const sqltoast::datetime_value_expression_t& sub =
                    static_cast<const sqltoast::datetime_value_expression_t&>(ve);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::VALUE_EXPRESSION_TYPE_INTERVAL_EXPRESSION:
            out << "INTERVAL_EXPRESSION";
            {
                const sqltoast::interval_value_expression_t& sub =
                    static_cast<const sqltoast::interval_value_expression_t&>(ve);
                to_yaml(ptr, out, sub);
            }
            break;
    }
    ptr.indent_pop(out);
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::numeric_expression_t& ne) {
    ptr.indent(out) << "numeric_expression:";
    ptr.indent_push(out);
    if ((! ne.right) && (! ne.left->right))
        to_yaml(ptr, out, *ne.left);
    else {
        ptr.indent(out) << "left:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *ne.left);
        ptr.indent_pop(out);
        if (ne.right) {
            if (ne.op == sqltoast::NUMERIC_OP_ADD)
                ptr.indent(out) << "op: ADD";
            else
                ptr.indent(out) << "op: SUBTRACT";
            ptr.indent(out) << "right:";
            ptr.indent_push(out);
            to_yaml(ptr, out, *ne.right);
            ptr.indent_pop(out);
        }
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::numeric_term_t& term) {
    ptr.indent(out) << "term:";
    ptr.indent_push(out);
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *term.left);
    ptr.indent_pop(out);
    if (term.right) {
        if (term.op == sqltoast::NUMERIC_OP_MULTIPLY)
            ptr.indent(out) << "op: MULTIPLY";
        else
            ptr.indent(out) << "op: DIVIDE";
        ptr.indent(out) << "right:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *term.right);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::numeric_factor_t& factor) {
    ptr.indent(out) << "factor:";
    ptr.indent_push(out);
    if (factor.sign != 0)
        ptr.indent(out) << "sign: " << factor.sign;
    to_yaml(ptr, out, *factor.primary);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::numeric_primary_t& primary) {
    ptr.indent(out) << "primary:";
    ptr.indent_push(out);
    ptr.indent(out) << "type: ";
    switch (primary.type) {
        case sqltoast::NUMERIC_PRIMARY_TYPE_VALUE:
            out << "VALUE";
            {
                const sqltoast::numeric_value_t& sub =
                    static_cast<const sqltoast::numeric_value_t&>(primary);
                ptr.indent(out) << "value:";
                ptr.indent_push(out);
                to_yaml(ptr, out, sub);
                ptr.indent_pop(out);
            }
            break;
        case sqltoast::NUMERIC_PRIMARY_TYPE_FUNCTION:
            out << "FUNCTION";
            {
                const sqltoast::numeric_function_t& sub =
                    static_cast<const sqltoast::numeric_function_t&>(primary);
                ptr.indent(out) << "function:";
                ptr.indent_push(out);
                to_yaml(ptr, out, sub);
                ptr.indent_pop(out);
            }
            break;
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::numeric_value_t& value) {
    ptr.indent(out) << "primary:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *value.primary);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::value_expression_primary_t& primary) {
    switch (primary.vep_type) {
        case sqltoast::VEP_TYPE_UNSIGNED_VALUE_SPECIFICATION:
            ptr.indent(out) << "type: UNSIGNED_VALUE_SPECIFICATION";
            {
                const sqltoast::unsigned_value_specification_t& sub =
                    static_cast<const sqltoast::unsigned_value_specification_t&>(primary);
                ptr.indent(out) << "unsigned_value_specification: " << sub;
            }
            break;
        case sqltoast::VEP_TYPE_COLUMN_REFERENCE:
            ptr.indent(out) << "type: COLUMN_REFERENCE";
            ptr.indent(out) << "column_reference: " << primary.lexeme;
            break;
        case sqltoast::VEP_TYPE_SET_FUNCTION_SPECIFICATION:
            ptr.indent(out) << "type: SET_FUNCTION_SPECIFICATION";
            {
                const sqltoast::set_function_t& sub =
                    static_cast<const sqltoast::set_function_t&>(primary);
                ptr.indent(out) << "set_function_specification: " << sub;
            }
            break;
        case sqltoast::VEP_TYPE_PARENTHESIZED_VALUE_EXPRESSION:
            ptr.indent(out) << "type: PARENTHESIZED_VALUE_EXPRESSION";
            {
                const sqltoast::parenthesized_value_expression_t& sub =
                    static_cast<const sqltoast::parenthesized_value_expression_t&>(primary);
                ptr.indent(out) << "parenthesized_value_expression: " << *sub.value;
            }
            break;
        case sqltoast::VEP_TYPE_CASE_EXPRESSION:
            ptr.indent(out) << "type: CASE_EXPRESSION";
            {
                const sqltoast::case_expression_t& sub =
                    static_cast<const sqltoast::case_expression_t&>(primary);
                ptr.indent(out) << "case_expression: " << sub;
            }
            break;
        case sqltoast::VEP_TYPE_SCALAR_SUBQUERY:
            ptr.indent(out) << "type: SCALAR_SUBQUERY";
            {
                const sqltoast::scalar_subquery_t& sub =
                    static_cast<const sqltoast::scalar_subquery_t&>(primary);
                ptr.indent(out) << "scalar_subquery: " << *sub.subquery;
            }
            break;
        default:
            break;
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::numeric_function_t& func) {
    bool found_length_func = false;
    ptr.indent(out) << "type: ";
    switch (func.type) {
        case sqltoast::NUMERIC_FUNCTION_TYPE_POSITION:
            out << "POSITION";
            {
                const sqltoast::position_expression_t& sub =
                    static_cast<const sqltoast::position_expression_t&>(func);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::NUMERIC_FUNCTION_TYPE_EXTRACT:
            out << "EXTRACT";
            {
                const sqltoast::extract_expression_t& sub =
                    static_cast<const sqltoast::extract_expression_t&>(func);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::NUMERIC_FUNCTION_TYPE_CHAR_LENGTH:
            out << "CHAR_LENGTH";
            found_length_func = true;
            break;
        case sqltoast::NUMERIC_FUNCTION_TYPE_BIT_LENGTH:
            out << "BIT_LENGTH";
            found_length_func = true;
            break;
        case sqltoast::NUMERIC_FUNCTION_TYPE_OCTET_LENGTH:
            out << "OCTET_LENGTH";
            found_length_func = true;
            break;
        default:
            // TODO(jaypipes)
            break;
    }
    if (found_length_func) {
        const sqltoast::length_expression_t& sub =
            static_cast<const sqltoast::length_expression_t&>(func);
        to_yaml(ptr, out, sub);
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::length_expression_t& expr) {
    ptr.indent(out) << "operand: " << *expr.operand;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::position_expression_t& expr) {
    ptr.indent(out) << "find: " << *expr.to_find;
    ptr.indent(out) << "in: " << *expr.subject;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::extract_expression_t& expr) {
    ptr.indent(out) << "field: " << expr.extract_field;
    ptr.indent(out) << "source: " << *expr.extract_source;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::character_value_expression_t& cve) {
    ptr.indent(out) << "character_expression:";
    ptr.indent_push(out);
    ptr.indent(out) << "factors:";
    ptr.indent_push(out);
    for (const std::unique_ptr<sqltoast::character_factor_t>& factor : cve.values) {
        ptr.start_list(out);
        to_yaml(ptr, out, *factor);
    }
    ptr.indent_pop(out);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::character_factor_t& factor) {
    bool is_list = ptr.in_list(out);
    if (is_list) {
        ptr.indent(out) << "- primary:";
        ptr.end_list(out);
        ptr.indent_push(out);
    } else {
        ptr.indent(out) << "primary:";
    }
    ptr.indent_push(out);
    to_yaml(ptr, out, *factor.primary);
    ptr.indent_pop(out);
    if (factor.collation)
        ptr.indent(out) << "collation: " << factor.collation;
    if (is_list)
        ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::character_primary_t& primary) {
    if (primary.value)
        ptr.indent(out) << "value: " << *primary.value;
    else
        to_yaml(ptr, out, *primary.string_function);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::string_function_t& func) {
    ptr.indent(out) << "function:";
    ptr.indent_push(out);
    ptr.indent(out) << "type: ";
    switch (func.type) {
        case sqltoast::STRING_FUNCTION_TYPE_SUBSTRING:
            out << "SUBSTRING";
            ptr.indent(out) << "operand:";
            ptr.indent_push(out);
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(*func.operand);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            {
                const sqltoast::substring_function_t& sub =
                    static_cast<const sqltoast::substring_function_t&>(func);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STRING_FUNCTION_TYPE_UPPER:
            out << "UPPER";
            ptr.indent(out) << "operand:";
            ptr.indent_push(out);
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(*func.operand);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            break;
        case sqltoast::STRING_FUNCTION_TYPE_LOWER:
            out << "LOWER";
            ptr.indent(out) << "operand:";
            ptr.indent_push(out);
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(*func.operand);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            break;
        case sqltoast::STRING_FUNCTION_TYPE_CONVERT:
            out << "CONVERT";
            ptr.indent(out) << "operand:";
            ptr.indent_push(out);
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(*func.operand);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            {
                const sqltoast::convert_function_t& sub =
                    static_cast<const sqltoast::convert_function_t&>(func);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STRING_FUNCTION_TYPE_TRANSLATE:
            out << "TRANSLATE";
            ptr.indent(out) << "operand:";
            ptr.indent_push(out);
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(*func.operand);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            {
                const sqltoast::translate_function_t& sub =
                    static_cast<const sqltoast::translate_function_t&>(func);
                to_yaml(ptr, out, sub);
            }
            break;
        case sqltoast::STRING_FUNCTION_TYPE_TRIM:
            out << "TRIM";
            ptr.indent(out) << "operand:";
            ptr.indent_push(out);
            {
                const sqltoast::character_value_expression_t& sub =
                    static_cast<const sqltoast::character_value_expression_t&>(*func.operand);
                to_yaml(ptr, out, sub);
            }
            ptr.indent_pop(out);
            {
                const sqltoast::trim_function_t& sub =
                    static_cast<const sqltoast::trim_function_t&>(func);
                to_yaml(ptr, out, sub);
            }
            break;
        default:
            out << "string-function[UNKNOWN]";
            break;
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::substring_function_t& func) {
    ptr.indent(out) << "start_position:";
    ptr.indent_push(out);
    const sqltoast::numeric_expression_t& start_pos_val =
        static_cast<sqltoast::numeric_expression_t&>(*func.start_position_value);
    to_yaml(ptr, out, start_pos_val);
    ptr.indent_pop(out);
    if (func.for_length_value) {
        ptr.indent(out) << "for_length:";
        ptr.indent_push(out);
        const sqltoast::numeric_expression_t& for_length_val =
            static_cast<sqltoast::numeric_expression_t&>(*func.for_length_value);
        to_yaml(ptr, out, for_length_val);
        ptr.indent_pop(out);
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::convert_function_t& func) {
    ptr.indent(out) << "using: " << func.conversion_name;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::translate_function_t& func) {
    ptr.indent(out) << "using: " << func.translation_name;
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::trim_function_t& func) {
    if (func.trim_character) {
        ptr.indent(out) << "specification: ";
        switch (func.specification) {
            case sqltoast::TRIM_SPECIFICATION_LEADING:
                out << "LEADING";
                break;
            case sqltoast::TRIM_SPECIFICATION_TRAILING:
                out << "TRAILING";
                break;
            case sqltoast::TRIM_SPECIFICATION_BOTH:
                out << "BOTH";
                break;
        }
        ptr.indent(out) << "trim_character:";
        ptr.indent_push(out);
        const sqltoast::character_value_expression_t& trim_char_val =
            static_cast<sqltoast::character_value_expression_t&>(*func.trim_character);
        to_yaml(ptr, out, trim_char_val);
        ptr.indent_pop(out);
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::datetime_value_expression_t& de) {
    ptr.indent(out) << "datetime_expression:";
    ptr.indent_push(out);
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *de.left);
    ptr.indent_pop(out);
    if (de.right) {
        if (de.op == sqltoast::NUMERIC_OP_ADD)
            ptr.indent(out) << "op: ADD";
        else
            ptr.indent(out) << "op: SUBTRACT";
        ptr.indent(out) << "right:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *de.right);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::datetime_term_t& term) {
    ptr.indent(out) << "term:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *term.value);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::datetime_factor_t& factor) {
    ptr.indent(out) << "factor:";
    ptr.indent_push(out);
    if (factor.is_local_tz())
        ptr.indent(out) << "time_zone: LOCAL";
    else
        ptr.indent(out) << "time_zone: " << factor.tz;
    to_yaml(ptr, out, *factor.primary);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::datetime_primary_t& primary) {
    ptr.indent(out) << "primary:";
    ptr.indent_push(out);
    ptr.indent(out) << "type: ";
    switch (primary.type) {
        case sqltoast::DATETIME_PRIMARY_TYPE_VALUE:
            out << "VALUE";
            ptr.indent(out) << "value: ";
            {
                const sqltoast::datetime_value_t& sub =
                    static_cast<const sqltoast::datetime_value_t&>(primary);
                out << sub;
            }
            break;
        case sqltoast::DATETIME_PRIMARY_TYPE_FUNCTION:
            out << "FUNCTION";
            ptr.indent(out) << "function: ";
            {
                const sqltoast::current_datetime_function_t& sub =
                    static_cast<const sqltoast::current_datetime_function_t&>(primary);
                out << sub;
            }
            break;
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::datetime_field_t& field) {
    ptr.indent(out) << "interval: " << field.interval;
    if (field.interval != sqltoast::INTERVAL_UNIT_SECOND) {
        if (field.precision > 0)
            ptr.indent(out) << "leading_precision: " << field.precision;
    } else {
        // We can have 0 leading precision and non-zero fractional precision
        // for second intervals...
        if (field.precision > 0 ||
                (field.precision == 0 && field.fractional_precision > 0)) {
            ptr.indent(out) << "leading_precision: " << field.precision;
            if (field.fractional_precision > 0)
                ptr.indent(out) << "fractional_precision: " << field.fractional_precision;
        }
    }
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::interval_value_expression_t& expr) {
    ptr.indent(out) << "interval_expression:";
    ptr.indent_push(out);
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *expr.left);
    ptr.indent_pop(out);
    if (expr.right) {
        if (expr.op == sqltoast::NUMERIC_OP_ADD)
            ptr.indent(out) << "op: ADD";
        else
            ptr.indent(out) << "op: SUBTRACT";
        ptr.indent(out) << "right:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *expr.right);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::interval_term_t& term) {
    ptr.indent(out) << "term:";
    ptr.indent_push(out);
    ptr.indent(out) << "left:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *term.left);
    ptr.indent_pop(out);
    if (term.right) {
        if (term.op == sqltoast::NUMERIC_OP_MULTIPLY)
            ptr.indent(out) << "op: MULTIPLY";
        else
            ptr.indent(out) << "op: DIVIDE";
        ptr.indent(out) << "right:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *term.right);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::interval_factor_t& factor) {
    ptr.indent(out) << "factor:";
    ptr.indent_push(out);
    if (factor.sign != 0)
        ptr.indent(out) << "sign: " << factor.sign;
    to_yaml(ptr, out, *factor.primary);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::interval_primary_t& primary) {
    ptr.indent(out) << "primary:";
    ptr.indent_push(out);
    ptr.indent(out) << "value:";
    ptr.indent_push(out);
    to_yaml(ptr, out, *primary.value);
    ptr.indent_pop(out);
    if (primary.qualifier)
        to_yaml(ptr, out, *primary.qualifier);
    ptr.indent_pop(out);
}

void to_yaml(printer_t& ptr, std::ostream& out, const sqltoast::interval_qualifier_t& qualifier) {
    ptr.indent(out) << "qualifier:";
    ptr.indent_push(out);
    ptr.indent(out) << "start:";
    ptr.indent_push(out);
    to_yaml(ptr, out, qualifier.start);
    ptr.indent_pop(out);
    if (qualifier.end) {
        ptr.indent(out) << "end:";
        ptr.indent_push(out);
        to_yaml(ptr, out, *qualifier.end);
        ptr.indent_pop(out);
    }
    ptr.indent_pop(out);
}

} // namespace sqltoast::print
} // namespace sqltoast
