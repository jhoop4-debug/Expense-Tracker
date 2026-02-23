using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace ExpenseTracker
{
    class Program
    {
        static void Main(string[] args)
        {
            ExpenseManager manager = new ExpenseManager();
            manager.LoadFromFile();

            bool exit = false;

            while (!exit)
            {
                Console.WriteLine("\n==== Simple Expense Tracker ====");
                Console.WriteLine("1. Add Expense");
                Console.WriteLine("2. View Expenses");
                Console.WriteLine("3. View Total Spent");
                Console.WriteLine("4. Save and Exit");
                Console.Write("Select option: ");

                string input = Console.ReadLine();

                switch (input)
                {
                    case "1":
                        manager.AddExpense();
                        break;
                    case "2":
                        manager.DisplayExpenses();
                        break;
                    case "3":
                        manager.DisplayTotal();
                        break;
                    case "4":
                        manager.SaveToFile();
                        exit = true;
                        break;
                    default:
                        Console.WriteLine("Invalid option. Try again.");
                        break;
                }
            }
        }
    }

    // Represents a single expense
    class Expense
    {
        public string Category { get; set; }
        public double Amount { get; set; }
        public DateTime Date { get; set; }

        public Expense(string category, double amount)
        {
            Category = category;
            Amount = amount;
            Date = DateTime.Now;
        }

        // Format for displaying to console
        public override string ToString()
        {
            return $"{Date.ToShortDateString()} - {Category}: ${Amount}";
        }

        // Format for saving to file
        public string ToFileFormat()
        {
            return $"{Date}|{Category}|{Amount}";
        }

        public static Expense FromFileFormat(string line)
        {
            string[] parts = line.Split('|');

            Expense expense = new Expense(parts[1], double.Parse(parts[2]));
            expense.Date = DateTime.Parse(parts[0]);

            return expense;
        }
    }

    class ExpenseManager
    {
        private List<Expense> expenses = new List<Expense>();
        private string filePath = "expenses.txt";

        public void AddExpense()
        {
            Console.Write("Enter category: ");
            string category = Console.ReadLine();

            Console.Write("Enter amount: ");
            string amountInput = Console.ReadLine();

            if (double.TryParse(amountInput, out double amount))
            {
                Expense newExpense = new Expense(category, amount);
                expenses.Add(newExpense);
                Console.WriteLine("Expense added.");
            }
            else
            {
                Console.WriteLine("Invalid amount.");
            }
        }

        public void DisplayExpenses()
        {
            if (expenses.Count == 0)
            {
                Console.WriteLine("No expenses recorded.");
                return;
            }

            Console.WriteLine("\n--- Expenses ---");
            foreach (Expense e in expenses)
            {
                Console.WriteLine(e);
            }
        }

        public void DisplayTotal()
        {
            double total = expenses.Sum(e => e.Amount);
            Console.WriteLine($"\nTotal spent: ${total}");
        }

        public void SaveToFile()
        {
            using (StreamWriter writer = new StreamWriter(filePath))
            {
                foreach (Expense e in expenses)
                {
                    writer.WriteLine(e.ToFileFormat());
                }
            }

            Console.WriteLine("Data saved to file.");
        }

        public void LoadFromFile()
        {
            if (!File.Exists(filePath))
                return;

            string[] lines = File.ReadAllLines(filePath);

            foreach (string line in lines)
            {
                expenses.Add(Expense.FromFileFormat(line));
            }
        }
    }
}
