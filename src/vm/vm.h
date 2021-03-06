//
// Created by jakevin on 2020/10/19.
//

#ifndef BEANDB_VM_H
#define BEANDB_VM_H


namespace vm {
    struct StorageEngine {
        buffer::BufferPoolManager *buffer_pool_manager_;
    };


    class ConsoleReader {
        ConsoleReader(const ConsoleReader &) = delete;

        ConsoleReader(ConsoleReader &&) = delete;

        ConsoleReader &operator=(const ConsoleReader &) = delete;

        ConsoleReader &operator=(ConsoleReader &&) = delete;

    public:
        ConsoleReader() = default;

        void start(std::future<void> exit_signal);

        void stop();

        std::string get_sql(); // might stuck
        void add_sql(std::string); // only used when redo
    private:
        std::queue<std::string> sql_pool_;
        std::thread reader_;
        std::mutex sql_pool_mutex_;
        std::condition_variable sql_pool_cv_;
    };


    class VM {
        friend class disk::DiskManager;

    public:

        VM();

        ~VM();

        VM(const VM &) = delete;

        VM(VM &&) = delete;

        VM &operator=(const VM &) = delete;

        VM &operator=(VM &&) = delete;

        void init();

        // run db task until user input "exit"
        void start();

        template<typename F, typename... Args>
        [[nodiscard]]
        std::future<std::invoke_result_t<F, Args...>> register_task(F &&f, Args &&...args) {
            return task_pool_.register_for_execution(std::forward<F>(f), std::forward<Args>(args)...);
        }

        void set_next_free_page_id(page::page_id_t);

        std::optional<table::TableInfo> getTableInfo(const std::string &tableName);

        std::future<void> get_exit_signal_for_console();

    private:

        void set_exit_signal_for_console();

        void send_reply_sql(std::string);

        struct process_result_t {
            bool exit = false;
            bool error = false;
            std::string msg;
        };

        process_result_t txn_process(const query::TPValue &);

        process_result_t query_process(query::APValue &);

        void doWAL(page::page_id_t prev_last_page_id, const std::string &sql);

        // flush dirty page into disk.
        void flush();

        void detroy_log();

        // return   OK   : no need to reply log.
        //          UNDO : undo the last txn.
        //          REDO : undo the last txn, then redo.
        disk::log_state_t check_log();

        void replay_log(disk::log_state_t);


        // txn process functions
        void doCreate(process_result_t &, const query::CreateTableInfo &);

        void doDrop(process_result_t &, const query::DropTableInfo &);

        void doSelect(process_result_t &, const query::TPSelectInfo &);

        void doUpdate(process_result_t &, const query::UpdateInfo &);

        void doInsert(process_result_t &, const query::InsertInfo &);

        void doDelete(process_result_t &, const query::DeleteInfo &);

        // query process function
        void doQuery(process_result_t &, query::APSelectInfo &);


        // 4 kinds of op node
        // - scanTable
        // - join
        // - projection
        // - sigma
    public:
        //friend struct TPTableOp;
        VirtualTable scanTable(const std::string &tableName);

        void doScanTable(VirtualTable ret, const std::string tableName);

        //friend struct TPJoinOp;
        // if JOIN ON PK, ignore the second pk name
        // uint32_t table2_col_start    denotes (t1.col, t1.col, t2.col, t2.col)
        // uint32_t vEntry_offset       denotes the t2 rows' offset after t1 rows
        VirtualTable join(VirtualTable t1, VirtualTable t2, bool pk);

        void doJoin(VirtualTable ret, VirtualTable t1, VirtualTable t2, bool pk,
                    uint32_t table2_col_start, uint32_t vEntry_offset);

        //friend struct TPProjectOp;
        VirtualTable projection(VirtualTable t, const std::vector<std::string> &colNames);

        void doProjection(VirtualTable ret, VirtualTable t, const std::vector<page::range_t> origin);

        //friend struct TPFilterOp;
        VirtualTable sigma(VirtualTable t, std::shared_ptr<ast::BaseExpr>);

        void doSigma(VirtualTable ret, VirtualTable t, std::shared_ptr<ast::BaseExpr>);


        void init_pk_view();

        void add_sql(std::string);

        // for mode switch
        void switch_mode();

        void AP_INIT();

        void AP_RESET();

        uint32_t get_ap_table_index(const std::string &) const;

    private:

        StorageEngine storage_engine_;
        util::ThreadsPool task_pool_;
        ConsoleReader console_reader_;
        std::promise<void> exit_signal_; // to inform console to stop from outside
        page::DBMetaPage *db_meta_;
        std::unordered_map<std::string, page::TableMetaPage *> table_meta_;
        // bad design to store `table::TableInfo` here, since need to update twice
        // infrastructure service is imperfect and we are not willing to be troubled with meta-info preparation and conversion
        // maybe we can replace it by providing `page::TableMetaPage*` -> `table::TableInfo`
        // In short, not good at all.
        std::unordered_map<std::string, table::TableInfo> table_info_;
        std::unordered_map<std::string, page::TableMetaPage *> free_table_;


        // PK view: pk -> ref
        // if the PK is not ref by any FK, the ref = 1;
        friend struct table::TableInfo;
        static constexpr uint32_t NON_FK_REF = 1;
        std::unordered_map<page::page_id_t,
        std::unordered_map<int32_t, uint32_t>> table_pk_ref_INT;
        std::unordered_map<page::page_id_t,
        std::unordered_map<std::string, uint32_t>> table_pk_ref_VARCHAR;


        // for AP
        bool tp_ = true;
        std::shared_ptr<ap::ap_table_array_t> ap_table_array_;
        std::vector<std::string> table_names_;


    public: // for test
        void test_create_table();

        uint32_t test_insert(const tree::KVEntry &);

        uint32_t test_erase(const page::KeyEntry &);

        page::ValueEntry test_find(const page::KeyEntry &);

        void test_size();

        void test_output();

        void test_flush();

        void showDB();

        void showSCHEMA();

        bool output_line_start = true;

        template<typename ...Arg>
        void query_print(const char *format, Arg... args) {
            if (output_line_start) {
                std::printf(">>> ");
                output_line_start = false;
            }
            std::printf(format, args...);
        }

        void query_print(std::string_view sv) {
            if (output_line_start) {
                std::printf(">>> ");
                output_line_start = false;
            }
            std::cout << sv;
        }

        void query_print_n();

        void println();

    };
};


#endif //BEANDB_VM_H
