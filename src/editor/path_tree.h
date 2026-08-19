
#ifdef TOOLS_ENABLED
#include <godot_cpp/core/print_string.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;
template <typename Data> class PathTree
{

  private:
    struct BranchNode;
    struct DataNode;

    struct Node
    {
      public:
        virtual DataNode *add(const String &p_name, Data p_data) = 0;
        virtual BranchNode *add(const String &p_name) = 0;

        Node(const String &p_name, bool p_has_children);
        String name;
        const bool has_children;
        virtual const Node *find(const String &p_name) const = 0;
        virtual Node *find(const String &p_name) = 0;
        virtual PackedStringArray get_contents() const = 0;
        Node()
        {
        }
        virtual ~Node()
        {
        }
    };

    struct BranchNode : Node
    {
      public:
        BranchNode(const String &p_name) : Node(p_name, true)
        {
        }
        virtual ~BranchNode();
        Vector<Node *> children;
        DataNode *add(const String &p_name, Data p_data) override;
        BranchNode *add(const String &p_name) override;
        const Node *find(const String &p_name) const override;
        Node *find(const String &p_name) override;

        PackedStringArray get_contents() const override;
    };
    struct DataNode : Node
    {
      public:
        DataNode(const String &p_name, Data p_data) : Node(p_name, false)
        {
            data = p_data;
        }
        Data data;
        DataNode *add(const String &p_name, Data p_data) override;
        BranchNode *add(const String &p_name) override;
        const Node *find(const String &p_name) const override;
        Node *find(const String &p_name) override;
        PackedStringArray get_contents() const override;
    };

  public:
    struct TreeItem
    {
        Node *node;
        String get_name() const
        {
            return node->name;
        }
        bool has_children() const
        {
            return node->has_children;
        }
        const TreeItem *find(const String &p_name) const
        {
            return {node->find(p_name)};
        }
        TreeItem *find(const String &p_name)
        {
            return {node->find(p_name)};
        }
        Vector<TreeItem> get_contents()
        {
            return node->get_contents();
        }
    };

  private:
    Node *find_node(const String &p_path, bool p_last_is_data = false);
    const Node *find_node(const String &p_path) const;
    BranchNode root;

  public:
    PathTree(/* args */);
    virtual ~PathTree();

    Data &operator[](const String &p_path);
    Data operator[](const String &p_path) const;

    void add_data(const String &p_path, Data p_data);
    void add(const String &p_path);
    void clear();
    PackedStringArray get_contents(const String &p_path) const;
    TreeItem get_root() const;
};
template <typename Data>
typename PathTree<Data>::Node *PathTree<Data>::find_node(const String &p_path, bool p_last_is_data)
{
    Node *current_branch = &root;
    PackedStringArray split = p_path.split("/", false);
    Node *next_branch;
    for (int i = 0; i < split.size(); i++)
    {
        if (!current_branch)
        {
            return nullptr;
        }
        next_branch = current_branch->find(split[i]);
        if (!next_branch)
        {
            if (i == split.size() - 1 && p_last_is_data)
            {
                next_branch = current_branch->add(split[i], Data());
            }
            else
            {
                next_branch = current_branch->add(split[i]);
            }
        };
        current_branch = next_branch;
    }
    return current_branch;
}

template <typename Data> const typename PathTree<Data>::Node *PathTree<Data>::find_node(const String &p_path) const
{
    const Node *current_branch = &root;
    PackedStringArray split = p_path.split("/", false);
    const Node *next_branch;
    for (int i = 0; i < split.size(); i++)
    {
        next_branch = current_branch->find(split[i]);
        if (!next_branch)
        {
            return nullptr;
        };
        current_branch = next_branch;
    }
    return current_branch;
}
template <typename Data> PathTree<Data>::PathTree() : root("")
{
}

template <typename Data> PathTree<Data>::~PathTree()
{
}
template <typename Data> Data &PathTree<Data>::operator[](const String &p_path)
{
    return ((PathTree<Data>::DataNode *)find_node(p_path, true))->data;
}

template <typename Data> Data PathTree<Data>::operator[](const String &p_path) const
{
    const Node *node = find_node(p_path);
    if (!node)
    {
        print_error("no node at [", p_path, "]");
        return Data();
    }
    return ((PathTree<Data>::DataNode *)node)->data;
}

template <typename Data> void PathTree<Data>::add_data(const String &p_path, Data p_data)
{
    DataNode *node = (DataNode *)find_node(p_path, true);
    node->data = p_data;
}

template <typename Data> void PathTree<Data>::add(const String &p_path)
{
    find_node(p_path, false);
}

template <typename Data> void PathTree<Data>::clear()
{
    for (auto child : root.children)
    {
        memdelete(child);
    }
    root.children.resize(0);
}

template <typename Data> PackedStringArray PathTree<Data>::get_contents(const String &p_path) const
{
    if (p_path.begins_with("/"))
    {
        return root.get_contents();
    }
    else
    {
        const Node *node = find_node(p_path);
        if (node)
        {
            return node->get_contents();
        }
        else
        {
            return {};
        }
    }
}
template <typename Data> typename PathTree<Data>::TreeItem PathTree<Data>::get_root() const
{
    return {&root};
}
template <typename Data>
PathTree<Data>::Node::Node(const String &p_name, bool p_has_children) : has_children(p_has_children)
{
    this->name = p_name;
}

template <typename Data>
typename PathTree<Data>::DataNode *PathTree<Data>::DataNode::add(const String &p_name, Data p_data)
{
    return nullptr;
}

template <typename Data> typename PathTree<Data>::BranchNode *PathTree<Data>::DataNode::add(const String &p_name)
{
    return nullptr;
}

template <typename Data> const typename PathTree<Data>::Node *PathTree<Data>::DataNode::find(const String &p_name) const
{
    if (p_name.casecmp_to(Node::name) == 0)
    {
        return this;
    }
    else
    {
        return nullptr;
    }
}

template <typename Data> typename PathTree<Data>::Node *PathTree<Data>::DataNode::find(const String &p_name)
{
    if (p_name.casecmp_to(Node::name) == 0)
    {
        return this;
    }
    else
    {
        return nullptr;
    }
}

template <typename Data> PackedStringArray PathTree<Data>::DataNode::get_contents() const
{
    return {};
}

template <typename Data> PathTree<Data>::BranchNode::~BranchNode()
{
    for (int i = 0; i < children.size(); i++)
    {
        memdelete(children[i]);
    }
}

template <typename Data>
typename PathTree<Data>::DataNode *PathTree<Data>::BranchNode::add(const String &p_name, Data p_data)
{
    DataNode *node = memnew(DataNode(p_name, p_data));
    children.push_back(node);
    return node;
}

template <typename Data> typename PathTree<Data>::BranchNode *PathTree<Data>::BranchNode::add(const String &p_name)
{
    BranchNode *branch = memnew(BranchNode(p_name));
    children.push_back(branch);
    return branch;
}

template <typename Data>
const typename PathTree<Data>::Node *PathTree<Data>::BranchNode::find(const String &p_name) const
{
    for (auto child : children)
    {
        if (p_name.casecmp_to(child->name) == 0)
        {
            return child;
        }
    }
    return nullptr;
}

template <typename Data> typename PathTree<Data>::Node *PathTree<Data>::BranchNode::find(const String &p_name)
{
    for (auto child : children)
    {
        if (p_name.casecmp_to(child->name) == 0)
        {
            return child;
        }
    }
    return nullptr;
}

template <typename Data> PackedStringArray PathTree<Data>::BranchNode::get_contents() const
{
    PackedStringArray arr;
    for (Node *node : children)
    {
        arr.append(node->name);
    }

    return arr;
}

#endif
