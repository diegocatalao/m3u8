vim.opt_local.expandtab = true
vim.opt_local.shiftwidth = 2
vim.opt_local.tabstop = 2

vim.opt_local.colorcolumn = "100"
vim.b.virt_column_config = { virtcolumn = "100" }

if package.loaded['virt-column'] then
  vim.schedule(function()
    vim.cmd('edit')
  end)
end
