#pragma once
#include <memory>
#include <type_traits>
#include <functional>

namespace gui::utils {

// Умный указатель с подсчетом ссылок и уведомлением об удалении
template<typename T>
class ObservablePtr {
public:
    ObservablePtr() : ptr_(nullptr), onDelete_(nullptr) {}
    
    explicit ObservablePtr(T* ptr, std::function<void(T*)> onDelete = nullptr)
        : ptr_(ptr), onDelete_(onDelete) {}
    
    ~ObservablePtr() {
        reset();
    }
    
    ObservablePtr(const ObservablePtr& other) = delete;
    ObservablePtr& operator=(const ObservablePtr& other) = delete;
    
    ObservablePtr(ObservablePtr&& other) noexcept
        : ptr_(other.ptr_), onDelete_(std::move(other.onDelete_)) {
        other.ptr_ = nullptr;
    }
    
    ObservablePtr& operator=(ObservablePtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            onDelete_ = std::move(other.onDelete_);
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    void reset() {
        if (ptr_) {
            if (onDelete_) {
                onDelete_(ptr_);
            }
            delete ptr_;
            ptr_ = nullptr;
        }
    }
    
    T* get() const { return ptr_; }
    T* operator->() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    operator bool() const { return ptr_ != nullptr; }

private:
    T* ptr_;
    std::function<void(T*)> onDelete_;
};

// Пул объектов для эффективного переиспользования
template<typename T>
class ObjectPool {
public:
    explicit ObjectPool(size_t initialSize = 0) {
        for (size_t i = 0; i < initialSize; ++i) {
            freeObjects_.push_back(std::make_unique<T>());
        }
    }
    
    template<typename... Args>
    T* acquire(Args&&... args) {
        T* object = nullptr;
        if (!freeObjects_.empty()) {
            object = freeObjects_.back().release();
            freeObjects_.pop_back();
        } else {
            object = new T(std::forward<Args>(args)...);
        }
        usedObjects_.push_back(std::unique_ptr<T>(object));
        return object;
    }
    
    void release(T* object) {
        auto it = std::find_if(usedObjects_.begin(), usedObjects_.end(),
            [object](const auto& ptr) { return ptr.get() == object; });
        
        if (it != usedObjects_.end()) {
            freeObjects_.push_back(std::move(*it));
            usedObjects_.erase(it);
        }
    }
    
    void clear() {
        freeObjects_.clear();
        usedObjects_.clear();
    }
    
    size_t getFreeCount() const { return freeObjects_.size(); }
    size_t getUsedCount() const { return usedObjects_.size(); }

private:
    std::vector<std::unique_ptr<T>> freeObjects_;
    std::vector<std::unique_ptr<T>> usedObjects_;
};

// Фабрика объектов с регистрацией типов
class ObjectFactory {
public:
    template<typename Base>
    class Registry {
    public:
        template<typename T>
        void registerType(const std::string& typeName) {
            static_assert(std::is_base_of<Base, T>::value, "T must inherit from Base");
            creators_[typeName] = []() { return std::make_unique<T>(); };
        }
        
        std::unique_ptr<Base> create(const std::string& typeName) const {
            auto it = creators_.find(typeName);
            if (it != creators_.end()) {
                return it->second();
            }
            return nullptr;
        }
        
        bool isTypeRegistered(const std::string& typeName) const {
            return creators_.find(typeName) != creators_.end();
        }
        
        std::vector<std::string> getRegisteredTypes() const {
            std::vector<std::string> types;
            for (const auto& pair : creators_) {
                types.push_back(pair.first);
            }
            return types;
        }

    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<Base>()>> creators_;
    };
};

// Умный указатель с отложенной инициализацией
template<typename T>
class LazyPtr {
public:
    LazyPtr() : initialized_(false) {}
    
    template<typename F>
    explicit LazyPtr(F&& initializer)
        : initializer_(std::forward<F>(initializer)), initialized_(false) {}
    
    T* get() {
        ensureInitialized();
        return ptr_.get();
    }
    
    T* operator->() {
        ensureInitialized();
        return ptr_.get();
    }
    
    T& operator*() {
        ensureInitialized();
        return *ptr_;
    }
    
    operator bool() const {
        return initialized_ && ptr_;
    }
    
    void reset() {
        ptr_.reset();
        initialized_ = false;
    }

private:
    void ensureInitialized() {
        if (!initialized_) {
            if (initializer_) {
                ptr_ = initializer_();
            }
            initialized_ = true;
        }
    }
    
    std::unique_ptr<T> ptr_;
    std::function<std::unique_ptr<T>()> initializer_;
    bool initialized_;
};

// Наблюдатель за изменениями значения
template<typename T>
class Observable {
public:
    using Observer = std::function<void(const T&)>;
    using ObserverId = uint64_t;

    Observable() = default;
    explicit Observable(const T& value) : value_(value) {}

    // Adds an observer and returns an id that can be used to remove it later.
    ObserverId addObserver(Observer observer) {
        const ObserverId id = ++nextId_;
        observers_.emplace(id, std::move(observer));
        return id;
    }

    // Remove observer by id. Returns true if removed.
    bool removeObserver(ObserverId id) {
        return observers_.erase(id) > 0;
    }

    void clearObservers() {
        observers_.clear();
    }

    void setValue(const T& value) {
        if (value_ != value) {
            value_ = value;
            notify();
        }
    }

    const T& getValue() const { return value_; }

private:
    void notify() {
        // Copy observers in case an observer modifies the map during iteration
        auto copy = observers_; 
        for (const auto& pair : copy) {
            const auto& observer = pair.second;
            if (observer) observer(value_);
        }
    }

    T value_{};
    std::unordered_map<ObserverId, Observer> observers_;
    ObserverId nextId_ = 0;
};

} // namespace gui::utils