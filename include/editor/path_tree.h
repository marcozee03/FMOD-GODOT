
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <iostream>
#include <godot_cpp/core/print_string.hpp>
using namespace std;
template <typename Data>
class PathTree
{

private:
    struct BranchNode;
    struct DataNode;

    struct Node
    {
    public:
        virtual DataNode *add(const String &name, Data data) = 0;
        virtual BranchNode *add(const String &name) = 0;

        Node(const String &name, bool p_has_children);
        String name;
        const bool has_children;
        virtual const Node *find(const String &name) const = 0;
        virtual Node *find(const String &name) = 0;
        virtual void print(int indent_level) const = 0;
        virtual PackedStringArray get_contents() const = 0;
    };

    struct BranchNode : Node
    {
    public:
        BranchNode(const String &name) : Node(name, true) {}
        ~BranchNode();
        Vector<Node *> children;
        DataNode *add(const String &name, Data data) override;
        BranchNode *add(const String &name) override;
        const Node *find(const String &name) const override;
        Node *find(const String &name) override;
        void print(int indent_level) const override;

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
        DataNode *add(const String &name, Data data) override;
        BranchNode *add(const String &name) override;
        const Node *find(const String &name) const override;
        Node *find(const String &name) override;
        void print(int indent_level) const override;
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
        const TreeItem *find(const String &name) const
        {
            return {node->find(name)};
        }
        TreeItem *find(const String &name)
        {
            return {node->find(name)};
        }
        Vector<TreeItem> get_contents()
        {
            return node->get_contents();
        }
    };

private:
    Node *find_node(const String &path, bool lastIsData = false);
    const Node *find_node(const String &path) const;
    BranchNode root;

public:
    PathTree(/* args */);
    virtual ~PathTree();

    Data &operator[](const String &path);
    Data operator[](const String &path) const;

    void add_data(const String &path, Data data);
    void add(const String &path);
    void clear();
    void print() const;
    PackedStringArray get_contents(const String &path) const;
    TreeItem get_root() const;
};
template <typename Data>
typename PathTree<Data>::Node *PathTree<Data>::find_node(const String &path, bool last_is_data)
{
    Node *current_branch = &root;
    PackedStringArray split = path.split("/", false);
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
            if (i == split.size() - 1 && last_is_data)
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

template <typename Data>
const typename PathTree<Data>::Node *PathTree<Data>::find_node(const String &path) const
{
    const Node *current_branch = &root;
    PackedStringArray split = path.split("/", false);
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
template <typename Data>
PathTree<Data>::PathTree() : root("")
{
}

template <typename Data>
PathTree<Data>::~PathTree()
{
}
template <typename Data>
Data &PathTree<Data>::operator[](const String &path)
{
    return ((PathTree<Data>::DataNode *)find_node(path, true))
        ->data;
}

template <typename Data>
Data PathTree<Data>::operator[](const String &path) const
{
    const Node *node = find_node(path);
    if (!node)
    {
        print_error("no node at [", path, "]");
        return Data();
    }
    return ((PathTree<Data>::DataNode *)node)->data;
}

template <typename Data>
void PathTree<Data>::add_data(const String &path, Data data)
{
    DataNode *node = (DataNode *)find_node(path, true);
    node->data = data;
}

template <typename Data>
void PathTree<Data>::add(const String &path)
{
    find_node(path, false);
}

template <typename Data>
void PathTree<Data>::clear()
{
    for (auto child : root.children)
    {
        delete child;
    }
    root.children.resize(0);
}

template <typename Data>
void PathTree<Data>::print() const
{
    std::cout << root.name.utf8() << std::endl;
    for (auto child : root.children)
    {
        child->print(1);
    }
}
template <typename Data>
PackedStringArray PathTree<Data>::get_contents(const String &path) const
{
    if (path.begins_with("/"))
    {
        return root.get_contents();
    }
    else
    {
        const Node *node = find_node(path);
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
template <typename Data>
typename PathTree<Data>::TreeItem PathTree<Data>::get_root() const
{
    return {&root};
}
template <typename Data>
PathTree<Data>::Node::Node(const String &name, bool p_has_children) : has_children(p_has_children)
{
    this->name = name;
}

template <typename Data>
typename PathTree<Data>::DataNode *PathTree<Data>::DataNode::add(const String &name, Data data)
{
    return nullptr;
}

template <typename Data>
typename PathTree<Data>::BranchNode *PathTree<Data>::DataNode::add(const String &name)
{
    return nullptr;
}

template <typename Data>
const typename PathTree<Data>::Node *PathTree<Data>::DataNode::find(const String &name) const
{
    if (name.casecmp_to(Node::name) == 0)
    {
        return this;
    }
    else
    {
        return nullptr;
    }
}

template <typename Data>
typename PathTree<Data>::Node *PathTree<Data>::DataNode::find(const String &name)
{
    if (name.casecmp_to(Node::name) == 0)
    {
        return this;
    }
    else
    {
        return nullptr;
    }
}

template <typename Data>
void PathTree<Data>::DataNode::print(int indent_level) const
{
    for (int i = 0; i < indent_level; i++)
    {
        cout << " ";
    }
    cout << Node::name.utf8() << " " << data << endl;
}

template <typename Data>
PackedStringArray PathTree<Data>::DataNode::get_contents() const
{
    return {};
}

template <typename Data>
PathTree<Data>::BranchNode::~BranchNode()
{
    for (int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
}

template <typename Data>
typename PathTree<Data>::DataNode *PathTree<Data>::BranchNode::add(const String &name, Data data)
{
    DataNode *node = new DataNode(name, data);
    children.push_back(node);
    return node;
}

template <typename Data>
typename PathTree<Data>::BranchNode *PathTree<Data>::BranchNode::add(const String &name)
{
    BranchNode *branch = new BranchNode(name);
    children.push_back(branch);
    return branch;
}

template <typename Data>
const typename PathTree<Data>::Node *PathTree<Data>::BranchNode::find(const String &name) const
{
    for (auto child : children)
    {
        if (name.casecmp_to(child->name) == 0)
        {
            return child;
        }
    }
    return nullptr;
}

template <typename Data>
typename PathTree<Data>::Node *PathTree<Data>::BranchNode::find(const String &name)
{
    for (auto child : children)
    {
        if (name.casecmp_to(child->name) == 0)
        {
            return child;
        }
    }
    return nullptr;
}

template <typename Data>
PackedStringArray PathTree<Data>::BranchNode::get_contents() const
{
    PackedStringArray arr;
    for (Node *node : children)
    {
        arr.append(node->name);
    }

    return arr;
}

template <typename Data>
void PathTree<Data>::BranchNode::print(int indent_level) const
{
    for (int i = 0; i < indent_level; i++)
    {
        std::cout << " ";
    }
    std::cout << Node::name.utf8() << std::endl;
    for (auto child : children)
    {
        child->print(indent_level + 1);
    }
}
