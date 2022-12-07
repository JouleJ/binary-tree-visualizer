#include "data-structure.hpp"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace lib {

class CasualBinaryTree : public DataStructure {
  private:
    INode *root;

    class CasualBinaryTreeNodeConfig {
      public:
        CasualBinaryTreeNodeConfig(const std::initializer_list<int> &l) {
            assert(l.size() > 0);
            assert(l.size() <= 3);

            node_id = std::data(l)[0];
            assert(node_id >= 0);
            left_child_id = (l.size() > 1 ? std::data(l)[1] : -1);
            right_child_id = (l.size() > 2 ? std::data(l)[2] : -1);
        }

        int node_id;
        int left_child_id;
        int right_child_id;
    };

    void topsort(int v, int p,
                 const std::vector<CasualBinaryTreeNodeConfig> &conf,
                 std::unordered_map<int, int> &id_to_conf_pos,
                 std::vector<int> &color, std::vector<int> &order) {
        assert(v >= 0);
        assert(color[v] == 0);
        if (p == -1) {
            color[v] = 1;
        } else {
            color[v] = 2;
        }

        int Lch = id_to_conf_pos.at(conf[v].left_child_id);
        if (Lch != -1) {
            assert(color[Lch] != 2);
            if (color[Lch] == 0) {
                topsort(Lch, v, conf, id_to_conf_pos, color, order);
            } else /* color[Lch] == 1 */ {
                color[Lch] = 2;
            }
        }

        int Rch = id_to_conf_pos.at(conf[v].right_child_id);
        if (Rch != -1) {
            assert(color[Rch] != 2);
            if (color[Rch] == 0) {
                topsort(Rch, v, conf, id_to_conf_pos, color, order);
            } else /* color[Lch] == 1 */ {
                color[Rch] = 2;
            }
        }

        order.push_back(v);
    }

    CasualBinaryTree(const std::vector<CasualBinaryTreeNodeConfig> &conf) {
        root = nullptr;
        std::unordered_map<int, int> id_to_conf_pos;
        std::vector<int> color(conf.size(), 0);

        id_to_conf_pos[-1] = -1;
        for (size_t i = 0; i < conf.size(); i++) {
            // TO-DO: understandable errors
            assert(id_to_conf_pos.count(conf[i].node_id) == 0);
            id_to_conf_pos[conf[i].node_id] = i;
        }

        std::vector<int> order;
        for (size_t i = 0; i < conf.size(); i++) {
            if (color[i] == 0) {
                topsort(i, -1, conf, id_to_conf_pos, color, order);
            }
        }

        std::vector<std::unique_ptr<INode>> ptrs(conf.size());
        for (auto i : order) {
            int Lch = conf[i].left_child_id;
            int Rch = conf[i].right_child_id;
            ptrs[i] = MakeNode(
                QString::number(conf[i].node_id),
                (Lch == -1 ? nullptr : std::move(ptrs[id_to_conf_pos[Lch]])),
                (Rch == -1 ? nullptr : std::move(ptrs[id_to_conf_pos[Rch]])));
        }

        for (size_t i = 0; i < conf.size(); i++) {
            if (ptrs[i]) {
                assert(root == nullptr);
                root = ptrs[i].release();
            }
        }
    }

  public:
    CasualBinaryTree() { root = nullptr; }

    CasualBinaryTree(const std::vector<std::initializer_list<int>> &l) {
        std::vector<CasualBinaryTreeNodeConfig> conf;
        conf.reserve(l.size());
        for (const auto &i : l) {
            conf.emplace_back(i);
        }
        *this = CasualBinaryTree(conf);
    }

    INode *getRoot() const override { return root; }

    size_t getRequestSchemeCount() const override { return 0; }

    const RequestScheme *getRequestScheme(size_t idx) const override {
        static_cast<void>(idx);
        return nullptr;
    }

    int64_t executeRequest(const RequestScheme *requestScheme,
                           const int64_t *firstValue) override {
        static_cast<void>(requestScheme);
        static_cast<void>(firstValue);
        return 0;
    }
};

std::unique_ptr<DataStructure>
MakeBinaryTree(const std::vector<std::initializer_list<int>> &conf) {
    return std::make_unique<CasualBinaryTree>(conf);
}
} // namespace lib
