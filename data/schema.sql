-- Database schema for HTTP Proxy Learning System
-- Created: 2025-12-15

-- Users table for authentication and user management
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    login TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    role TEXT NOT NULL DEFAULT 'student',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Study progress tracking table
CREATE TABLE IF NOT EXISTS study_progress (
    user_id INTEGER NOT NULL,
    chapter_id INTEGER NOT NULL,
    status TEXT NOT NULL DEFAULT 'not_started',
    last_score INTEGER DEFAULT 0,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, chapter_id),
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- Create indexes for better performance
CREATE INDEX IF NOT EXISTS idx_users_login ON users(login);
CREATE INDEX IF NOT EXISTS idx_study_progress_user_id ON study_progress(user_id);
CREATE INDEX IF NOT EXISTS idx_study_progress_chapter_id ON study_progress(chapter_id);

-- Insert default admin user (password: admin123)
INSERT INTO users (login, password_hash, role) 
VALUES ('admin', '$2b$12$LQv3c1yqBWVHxkd0LHAkCOYz6TtxMQJqhN8/LewdBPj/RK.PZvO.G', 'admin')
ON CONFLICT (login) DO NOTHING;