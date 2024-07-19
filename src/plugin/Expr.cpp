#include <algorithm>
#include <ctime>
#include <functional>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace expr {
struct Expr {
    virtual ~Expr()                         = default;
    virtual std::vector<float> eval() const = 0;
};
struct BinaryOperatorExpr : Expr {
    Expr* a;
    Expr* b;
    BinaryOperatorExpr(Expr*, Expr*);
    ~BinaryOperatorExpr() override;
    virtual std::vector<float> eval() const override = 0;
};
struct AddExpr : BinaryOperatorExpr {
    AddExpr(Expr* a, Expr* b);
    virtual ~AddExpr() override;
    virtual std::vector<float> eval() const override;
};
struct SubExpr : BinaryOperatorExpr {
    SubExpr(Expr* a, Expr* b);
    virtual ~SubExpr() override;
    virtual std::vector<float> eval() const override;
};
struct MulExpr : BinaryOperatorExpr {
    MulExpr(Expr* a, Expr* b);
    virtual ~MulExpr() override;
    virtual std::vector<float> eval() const override;
};
struct DivExpr : BinaryOperatorExpr {
    DivExpr(Expr* a, Expr* b);
    virtual ~DivExpr() override;
    virtual std::vector<float> eval() const override;
};
struct TupleExpr : Expr {
    std::vector<float> data;
    TupleExpr(const std::vector<float>&);
    virtual ~TupleExpr() override;
    virtual std::vector<float> eval() const override;
};
struct ParamExpr : Expr {
    std::unordered_map<std::string, Expr*>* params;
    std::string                             id;
    ParamExpr(std::unordered_map<std::string, Expr*>*, const std::string&);
    virtual ~ParamExpr() override;
    virtual std::vector<float> eval() const override;
};
} // namespace expr

namespace expr {
BinaryOperatorExpr::BinaryOperatorExpr(Expr* a, Expr* b) : a(a), b(b) {}
BinaryOperatorExpr::~BinaryOperatorExpr() {
    delete a;
    delete b;
}
AddExpr::AddExpr(Expr* a, Expr* b) : BinaryOperatorExpr(a, b) {}
std::vector<float> AddExpr::eval() const {
    std::vector<float> res;
    auto               a_  = a->eval();
    auto               b_  = b->eval();
    auto               end = std::min(a_.size(), b_.size());
    res.resize(end);
    for (std::size_t i = 0; i < end; i++) {
        res[i] = a_[i] + b_[i];
    }
    return res;
}
AddExpr::~AddExpr() {}
SubExpr::SubExpr(Expr* a, Expr* b) : BinaryOperatorExpr(a, b) {}
std::vector<float> SubExpr::eval() const {
    std::vector<float> res;
    auto               a_  = a->eval();
    auto               b_  = b->eval();
    auto               end = std::min(a_.size(), b_.size());
    res.resize(end);
    for (std::size_t i = 0; i < end; i++) {
        res[i] = a_[i] - b_[i];
    }
    return res;
}
SubExpr::~SubExpr() {}
MulExpr::MulExpr(Expr* a, Expr* b) : BinaryOperatorExpr(a, b) {}
std::vector<float> MulExpr::eval() const {
    std::vector<float> res;
    auto               a_  = a->eval();
    auto               b_  = b->eval();
    auto               end = std::min(a_.size(), b_.size());
    res.resize(end);
    for (std::size_t i = 0; i < end; i++) {
        res[i] = a_[i] * b_[i];
    }
    return res;
}
MulExpr::~MulExpr() {}
DivExpr::DivExpr(Expr* a, Expr* b) : BinaryOperatorExpr(a, b) {}
std::vector<float> DivExpr::eval() const {
    std::vector<float> res;
    auto               a_  = a->eval();
    auto               b_  = b->eval();
    auto               end = std::min(a_.size(), b_.size());
    res.resize(end);
    for (std::size_t i = 0; i < end; i++) {
        res[i] = a_[i] / b_[i];
    }
    return res;
}
DivExpr::~DivExpr() {}
TupleExpr::TupleExpr(const std::vector<float>& data) : data(data) {}
std::vector<float> TupleExpr::eval() const { return data; }
TupleExpr::~TupleExpr() {}
ParamExpr::ParamExpr(std::unordered_map<std::string, Expr*>* params, const std::string& id) : params(params), id(id) {}
std::vector<float> ParamExpr::eval() const { return params->at(id)->eval(); }
ParamExpr::~ParamExpr() {}
std::vector<std::string> parse(const std::string& input) {
    using namespace std::string_literals;
    std::vector<std::string> cres;
    std::string              curr;
    for (auto c : input) {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ',' || c == ';' || c == '=') {
            if (!curr.empty()) {
                cres.push_back(curr);
                curr.clear();
            }
            cres.push_back(""s + c);
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!curr.empty()) {
                cres.push_back(curr);
                curr.clear();
            }
        } else {
            curr += c;
        }
    }
    if (!curr.empty()) cres.push_back(curr);
    return cres;
}

std::pair<Expr*, std::size_t>
eval_mul_or_div(std::vector<Expr*>& exprs, const std::vector<std::string>& ops, std::size_t begin) {
    Expr*       res  = exprs[begin];
    std::size_t stop = ops.size();
    for (auto i = begin; i < ops.size(); i++) {
        if (ops[i] == "+" || ops[i] == "-") {
            stop = i;
            break;
        } else if (ops[i] == "*") {
            res = new MulExpr(res, exprs[i + 1]);
        } else if (ops[i] == "/") {
            res = new DivExpr(res, exprs[i + 1]);
        }
    }
    return {res, stop};
}

Expr* eval_exprs(std::vector<Expr*>& exprs, const std::vector<std::string>& ops) {
    Expr*       res;
    std::size_t begin;
    if (exprs.size() == 1) return exprs[0];
    if (ops[0] == "*" || ops[0] == "/") {
        auto [r, s] = eval_mul_or_div(exprs, ops, 0);
        res         = r;
        begin       = s;
    } else {
        res   = exprs[0];
        begin = 0;
    }
    for (std::size_t i = begin; i < ops.size();) {

        if (i + 1 < ops.size()) {
            if (ops[i + 1] == "*" || ops[i + 1] == "/") {
                auto [r, s] = eval_mul_or_div(exprs, ops, i + 1);
                if (ops[i] == "+") {
                    res = new AddExpr(res, r);
                } else if (ops[i] == "-") {
                    res = new SubExpr(res, r);
                }
                i = s;
            } else {
                if (ops[i] == "+") {
                    res = new AddExpr(res, exprs[i + 1]);
                } else if (ops[i] == "-") {
                    res = new SubExpr(res, exprs[i + 1]);
                }
                i++;
            }
        } else {
            if (ops[i] == "+") {
                res = new AddExpr(res, exprs[i + 1]);
            } else if (ops[i] == "-") {
                res = new SubExpr(res, exprs[i + 1]);
            }
            i++;
        }
    }
    return res;
}
Expr* eval_single_code(
    const std::vector<std::string>&         expr,
    std::unordered_map<std::string, Expr*>* params,
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>>*
                functions,
    std::size_t begin = 0,
    std::size_t end   = std::numeric_limits<std::size_t>::max()
);
std::pair<Expr*, std::size_t> eval_function_call(
    const std::vector<std::string>&         expr,
    std::unordered_map<std::string, Expr*>* params,
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>>*
                functions,
    std::size_t begin,
    std::size_t end
) {
    int         ck   = 0;
    std::size_t stop = 0;
    for (auto i = begin + 1; i < end; i++) {
        if (expr[i] == "(") ck++;
        else if (expr[i] == ")") ck--;
        if (ck == 0) {
            stop = i;
            break;
        }
    }
    std::vector<std::vector<float>> args;
    std::size_t                     arg_begin = begin + 2;
    ck                                        = 0;
    for (auto i = begin + 2; i <= stop; i++) {
        if (i == stop) {
            if (arg_begin != stop) {
                auto r = eval_single_code(expr, params, functions, arg_begin, stop);
                args.push_back(r->eval());
                delete r;
            }
        } else if (expr[i] == "(") ck++;
        else if (expr[i] == ")") ck--;
        else if (expr[i] == "," && ck == 0) {
            auto r = eval_single_code(expr, params, functions, arg_begin, i);
            args.push_back(r->eval());
            delete r;
            arg_begin = i + 1;
        }
    }
    auto r = functions->at(expr[begin])(args);
    return {new TupleExpr(r), stop};
}
Expr* eval_single_code(
    const std::vector<std::string>&         expr,
    std::unordered_map<std::string, Expr*>* params,
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>>*
                functions,
    std::size_t begin,
    std::size_t end
) {
    if (end == std::numeric_limits<std::size_t>::max()) end = expr.size();
    if (end - begin == 2) {
        return new TupleExpr({std::stof(expr[begin] + expr[begin + 1])});
    }
    std::vector<Expr*>       cexprs;
    std::vector<std::string> ops;
    for (auto i = begin; i < end;) {
        if (expr[i] == "(") {
            int         ck   = 0;
            std::size_t stop = 0;
            for (auto j = i; j < end; j++) {
                if (expr[j] == "(") ck++;
                else if (expr[j] == ")") ck--;
                if (ck == 0) {
                    stop = j;
                    break;
                }
            }
            cexprs.push_back(eval_single_code(expr, params, functions, i + 1, stop));
            if (stop + 1 < end) {
                ops.push_back(expr[stop + 1]);
            }
            i = stop + 2;
        } else if ([&] {
                       for (auto c : expr[i])
                           if (!((c >= '0' && c <= '9') || c == '.')) return false;
                       return true;
                   }()) {
            cexprs.push_back(new TupleExpr({std::stof(expr[i])}));
            if (i + 1 < end) {
                ops.push_back(expr[i + 1]);
            }
            i += 2;
        } else {
            if (i + 1 < end && expr[i + 1] == "(") {
                auto [e, s] = eval_function_call(expr, params, functions, i, end);
                i           = s + 2;
                cexprs.push_back(e);
                if (s + 1 < end) {
                    ops.push_back(expr[s + 1]);
                }
            } else {
                cexprs.push_back(new ParamExpr(params, expr[i]));
                if (i + 1 < end) {
                    ops.push_back(expr[i + 1]);
                }
                i += 2;
            }
        }
    }
    return eval_exprs(cexprs, ops);
}

std::vector<float> eval(
    const std::string&                                                                                         code,
    const std::unordered_map<std::string, std::vector<float>>&                                                 params,
    std::unordered_map<std::string, std::function<std::vector<float>(const std::vector<std::vector<float>>&)>> functions
) {
    auto tokens        = parse(code);
    functions["tuple"] = [](const std::vector<std::vector<float>>& args) {
        std::vector<float> res;
        res.resize(static_cast<std::size_t>(args[0][0]));
        if (args.size() >= 2) {
            for (auto& t : res) t = args[1][0];
        }
        return res;
    };
    functions["subtuple"] = [](const std::vector<std::vector<float>>& args) {
        std::vector<float> res;
        res.resize(static_cast<std::size_t>(args[1][0] - args[0][0]));
        for (std::size_t i = static_cast<std::size_t>(args[0][0]); i < static_cast<std::size_t>(args[1][0]); i++) {
            res[i] = args[2][i];
        }
        return res;
    };
    functions["len"] = [](const std::vector<std::vector<float>>& args) {
        std::vector<float> res;
        res.resize(args.size());
        for (std::size_t i = 0; i < args.size(); i++) {
            res[i] = static_cast<float>(args[i].size());
        }
        return res;
    };
    functions["of"] = [](const std::vector<std::vector<float>>& args) {
        std::vector<float> res{args[1][static_cast<std::size_t>(args[0][0])]};
        return res;
    };
    functions["sum"] = [](const std::vector<std::vector<float>>& args) {
        float sum = 0;
        for (auto v : args[0]) sum += v;
        return std::vector<float>{sum};
    };
    functions["con"] = [](const std::vector<std::vector<float>>& args) {
        std::vector<float> res = {};
        for (auto i : args)
            for (auto j : i) res.push_back(j);
        res.shrink_to_fit();
        return res;
    };
    functions["max"] = [](const std::vector<std::vector<float>>& args) {
        return std::vector<float>{*std::max_element(args[0].begin(), args[0].end())};
    };
    functions["min"] = [](const std::vector<std::vector<float>>& args) {
        return std::vector<float>{*std::min_element(args[0].begin(), args[0].end())};
    };
    functions["sort"] = [](const std::vector<std::vector<float>>& args) {
        auto res = args[0];
        std::sort(res.begin(), res.end());
        return res;
    };
    static std::stack<std::vector<float>> stack;
    while (!stack.empty()) stack.pop();
    functions["push"] = [](const std::vector<std::vector<float>>& args) {
        stack.push(args[0]);
        return args[0];
    };
    functions["pop"] = [](const std::vector<std::vector<float>>&) {
        auto r = stack.top();
        stack.pop();
        return r;
    };
    std::unordered_map<std::string, Expr*> tparams;
    for (auto [id, v] : params) tparams[id] = new TupleExpr(v);
    std::vector<float> res;
    std::size_t        begin = 0;
    for (std::size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == ";") {
            auto f = eval_single_code(tokens, &tparams, &functions, begin, i);
            res    = f->eval();
            begin  = i + 1;
            delete f;
        }
    }

    for (auto [_, v] : tparams) delete v;
    return res;
}
} // namespace expr